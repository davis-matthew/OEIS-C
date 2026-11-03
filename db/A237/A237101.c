/*
 * A237101  Number of (n+1)X(1+1) 0..2 arrays with the maximum minus the upper
 *          median of every 2X2 subblock equal.
 * https://oeis.org/A237101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 188 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1)×2 arrays of 0..2 for which
 * the maximum minus the upper median of every 2×2 subblock is
 * the same constant across all subblocks.
 * This is computed by treating each row as a state of two
 * ternary digits and counting walks of length n through a
 * directed graph where an edge exists iff the pair of rows
 * yields a fixed difference. The total is the sum over the
 * three possible differences (0,1,2).  The implementation
 * uses matrix exponentiation over 9×9 matrices (9 possible rows).
 */
#include <stdlib.h>

/* compute maximum minus upper median of four values (0..2) */
static int diff_of_rows(const int r1[2], const int r2[2]) {
    int a = r1[0], b = r1[1], c = r2[0], d = r2[1];
    int freq[3] = {0,0,0};

    freq[a]++; freq[b]++; freq[c]++; freq[d]++;

    /* maximum value */
    int M = 2;
    if (freq[2] == 0) M = 1;
    if (freq[2] == 0 && freq[1] == 0) M = 0;

    /* second largest (upper median) */
    int second;
    if (freq[M] >= 2) {
        second = M;
    } else {
        /* max of values < M */
        second = M - 1;
        while (second >= 0 && freq[second] == 0) second--;
        if (second < 0) second = 0; /* safety, though should never happen */
    }
    return M - second; /* 0,1, or 2 */
}

#define ROWS 9

/* representation of a 9×9 matrix with entries of type seq_t */
typedef struct {
    seq_t m[ROWS][ROWS];
} Matrix;

/* Row values: each row is a pair (a,b) with a,b∈{0,1,2} */
static const int row_vals[ROWS][2] = {
    {0,0}, {0,1}, {0,2},
    {1,0}, {1,1}, {1,2},
    {2,0}, {2,1}, {2,2}
};

/* multiply two matrices over seq_t */
static void mat_mul(const Matrix *A, const Matrix *B, Matrix *C) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            C->m[i][j] = 0;
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int k = 0; k < ROWS; k++) {
            if (A->m[i][k] == 0) continue;
            seq_t aik = A->m[i][k];
            for (int j = 0; j < ROWS; j++) {
                C->m[i][j] += aik * B->m[k][j];
            }
        }
    }
}

/* identity matrix */
static void mat_identity(Matrix *I) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            I->m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

/* matrix exponentiation: res = A^exp */
static void mat_pow(const Matrix *A, idx_t exp, Matrix *res) {
    Matrix base = *A;
    Matrix result;
    mat_identity(&result);
    while (exp) {
        if (exp & 1) {
            Matrix temp;
            mat_mul(&result, &base, &temp);
            result = temp;
        }
        Matrix temp;
        mat_mul(&base, &base, &temp);
        base = temp;
        exp >>= 1;
    }
    *res = result;
}

/* global adjacency matrices for each possible difference */
static Matrix adj[3];
static int adj_initialized = 0;

/* initialize adjacency matrices */
static void init_adj(void) {
    for (int d = 0; d < 3; d++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < ROWS; j++) {
                adj[d].m[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            int d = diff_of_rows(row_vals[i], row_vals[j]);
            adj[d].m[i][j] = 1;
        }
    }
    adj_initialized = 1;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (!adj_initialized) init_adj();
    seq_t total = 0;
    for (int d = 0; d < 3; d++) {
        Matrix P;
        mat_pow(&adj[d], n, &P);   /* number of walks of length n */
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < ROWS; j++) {
                total += P.m[i][j];
            }
        }
    }
    return total;
}
