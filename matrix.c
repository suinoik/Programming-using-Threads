#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define MAX 4

int step=0;

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 


void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  int* coord_ptr = (int *)args;
  int j = coord_ptr[0];
  int k = coord_ptr[1];
  matSumResult[j][k] = matA[j][k] + matB[j][k];
  free(coord_ptr);
  pthread_exit(0);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matDiffResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  int* coord_ptr = (int *)args;
  int j = coord_ptr[0];
  int k = coord_ptr[1];
  matDiffResult[j][k] = matA[j][k] - matB[j][k];
  free(coord_ptr);
  pthread_exit(0);
}

// Calculates the value for each cell in matProductResult
void* computeProduct(void* args) { // pass in the number of the ith thread
  int i = step++;
  for (int j = 0; j < MAX; j++)
    for (int k = 0; k < MAX; k++)
      matProductResult[i][j] += matA[i][k] * matB[k][j];
  pthread_exit(0);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
  srand(time(0));  // Do Not Remove. Just ignore and continue below.
  
  // 0. Get the matrix size from the command line and assign it to MAX
  // printf("Enter the matrix size: ");

  
  // 1. Fill the matrices (matA and matB) with random values.
  fillMatrix(matA);
  fillMatrix(matB);
  
  // 2. Print the initial matrices.
  printf("Matrix A:\n");
  printMatrix(matA);
  printf("Matrix B:\n");
  printMatrix(matB);
  
  // 3. Create pthread_t objects for our threads.
  pthread_t threads_p[MAX];
  pthread_t threads_s[MAX*MAX];
  pthread_t threads_d[MAX*MAX];
  
  // 4. Create a thread for each cell of each matrix operation.
  int x, y;
  for (x=0; x<MAX; x++) {
    int* p;
    pthread_create(&threads_p[x], NULL, computeProduct, (void *)(p));
  }

  // 5. Wait for all threads to finish
  for (y=0; y<MAX; y++) {
    pthread_join(threads_p[y], NULL);
  }

  int a, b;
  
  
  for (a=0; a<MAX; a++) {
    for (b=0; b<MAX; b++) {
      int* coord_ptr_s = malloc(2*sizeof(int));
      coord_ptr_s[0] = a;
      coord_ptr_s[1] = b;
      pthread_create(&threads_s[a+b], NULL, computeSum, (void *)coord_ptr_s);
    }
  }

  for (y=0; y<MAX; y++) {
    pthread_join(threads_s[y], NULL);
  }
  
  for (a=0; a<MAX; a++) {
    for (b=0; b<MAX; b++) {
      int* coord_ptr_d = malloc(2*sizeof(int));
      coord_ptr_d[0] = a;
      coord_ptr_d[1] = b;
      pthread_create(&threads_d[a+b], NULL, computeDiff, (void *)coord_ptr_d);
    }
  }

  for (y=0; y<MAX; y++) {
    pthread_join(threads_d[y], NULL);
  }
  
  // 6. Print the results.
  printf("Results:\n");
  printf("Sum:\n");
  printMatrix(matSumResult);
  printf("Difference:\n");
  printMatrix(matDiffResult);
  printf("Product:\n");
  printMatrix(matProductResult);
  return 0;
  
}