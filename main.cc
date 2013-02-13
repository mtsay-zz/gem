/*
  For a better and more detailed explanation of how this solution works, see README
 */

#include<stdio.h>
#include<string.h>

#define MODULO 1000000007

int solve(int N, int K);

// framework skeleton
int main() {
  int T, N, K;

  scanf("%d", &T);

  for(int t = 0; t < T; t++) {
    scanf("%d %d\n", &N, &K);

    printf("%d\n", solve(N, K));
  }
}

// t1[0] := 1
// t1[N] := number of ways to make a 1x1xN block
int t1[6]; 

/*
  given an integer 0 <= N < 32, first express N in binary form
    for example, 13 = 01101, 
  then a bit of 0 indicates an open cell in the row
  whereas a bit of 1 indicates an occupied cell in the row

  table[X][Y] indicates the number of ways to go from a row of
    configuration X to configuration Y

  also doubles as the transition matrix and storage for the final
    matrix
 */
int table[32][32]; 

// initialize the lookup table T1
void initializeT1() {
  t1[0] = 1;
  t1[1] = 1;
  t1[2] = 2;

  for (int i = 3; i < 6; i++)
    t1[i] = t1[i - 1] + t1[i - 2];
}

// pre: K <= 5
// populates table[i][j]
int solveTableCell(int K, int i, int j) {
  if ((i & j) > 0)
    return 0;
  else {
    int soln = 1;
    int run = 0;

    for(int k = 0; k < K; k++, i = i >> 1, j = j >> 1) {
      if ((i | j) & 1) {
        soln *= t1[run];
        run = 0;
      } else
        run++;
    }

    soln *= t1[run];

    return soln;
  } 
}

// pre: K <= 5
// populates table/transition matrix
void initializeTable(int K) {
  for (int i = 0; i < (1 << K); i++)
    for (int j = 0; j < (1 << K); j++)
      table[i][j] = solveTableCell(K, i, j);
}

// given x, y, compute the index of the x-th row and y-th column on a 32x32 matrix
#define idx(x,y) ((x * 32) + y)

// square the table pointed by from and store the output to the table
//   pointed by to
void squareTable(int *from, int *to) {
  for (int i = 0; i < 32; i++)
    for (int j = 0; j < 32; j++) {
      long soln = 0;

      for (int k = 0; k < 32; k++) 
        soln = (soln + 1L * from[idx(i, k)] * from[idx(k, j)]) % MODULO;

      to[idx(i, j)] = (int) soln;
    }
}

// raise table to 2^N power
void exponentiateTable(int N) {
  int *p = new int[32 * 32];
  int *q = new int[32 * 32];

  memcpy(p, table, sizeof(table));

  for (int n = 0; n < N; n++) {
    squareTable(p, q);

    int *swap = q;
    q = p;
    p = swap;
  }

  memcpy(table, p, sizeof(table));

  delete[] p;
  delete[] q;
}

// for a better and more detailed explanation of how this solution
//   works, read about it in README
int solve(int N, int K) {
  // technically only needs to be done once, but it's fast anyway
  initializeT1();

  // it's important to clear the table every time!
  // since the next transition matrix could be smaller!
  memset(table, 0, sizeof(table));
  initializeTable(K);

  exponentiateTable(N);

  return table[0][0];
}

// the following functions were written to aid in debugging efforts

// prints the content of the lookup table T1
void printT1() {
  printf("T1: { ");

  for (int i = 0; i < 6; i++) {
    if (i > 0) printf(", ");
    printf("%d", t1[i]);
  }

  printf(" }\n");
}

// prints a configuration BACKWARDS
void printConfiguration(int n) {
  for (int i = 0; i < 5; i++, n = n >> 1)
    printf("%d", n & 1);
}

// prints the content of table
void printTable() {
  printf("Table:\n");

  for (int i = 0; i < 32; i++)
    for (int j = 0; j < 32; j++) {
      printConfiguration(i);
      printf(" >> ");
      printConfiguration(j);
      printf(" = %d\n", table[i][j]);
    }
}

// do a test run to see if the corresponding lookup
//   tables were populated correctly.
// since the value of these lookup table is quite
//   easy to compute for K <= 5, visual inspection
//   is sufficient to verify correctness.
void testRun() {
  initializeT1();
  initializeTable(5);

  printT1();
  printf("\n");
  printTable();
}

