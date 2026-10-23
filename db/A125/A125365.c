/*
 * A125365  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i−j| ≤ 4 (else 0); a(0)
 *            = 1. This counts base‑28 circular n‑digit numbers whose adjacent
 *            digits differ by at most 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define N 28

typedef struct { seq_t m[N][N]; } Matrix;

/* ------------------------------------------------------------------ */
/* Utility functions for matrix operations                           */
/* ------------------------------------------------------------------ */

static void matrix_identity(Matrix *R) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            R->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

static void matrix_multiply(const Matrix *A, const Matrix *B, Matrix *R) {
    /* R = A * B */
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < N; ++k)
                sum += A->m[i][k] * B->m[k][j];
            R->m[i][j] = sum;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Adjacency matrix for the graph where employees are adjacent if
   their digit difference is ≤ 4.                                    */
/* ------------------------------------------------------------------ */

static void adjacency_matrix(Matrix *M) {
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            M->m[i][j] = ((i > j ? (i - j <= 4) : (j - i <= 4)) ? (seq_t)1 : (seq_t)0);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Static adjacency matrix (computed once). */
    static Matrix baseAdj;
    static bool adjInit = false;
    if (!adjInit) {
        adjacency_matrix(&baseAdj);
        adjInit = true;
    }

    /* Binary exponentiation: compute baseAdj^n. */
    Matrix base = baseAdj;          /* copy, will be squared */
    Matrix result;
    matrix_identity(&result);       /* result = I */

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            Matrix tmp;
            matrix_multiply(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {                  /* avoid a useless square when done */
            Matrix tmp;
            matrix_multiply(&base, &base, &tmp);
            base = tmp;
        }
    }

    /* Sum of diagonal entries (trace). */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < N; ++i)
        sum += result.m[i][i];
    return sum;
}
