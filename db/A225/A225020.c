/*
 * A225020  Number of (n+1)X2 0..2 matrices with each 2X2 permanent equal.
 * https://oeis.org/A225020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A225020:
   Number of (n+1)×2 0..2 matrices with each 2×2 permanent equal.
   This implementation counts walks on a 9‑state graph whose edges
   are labelled by the 2×2 permanent of a pair of rows.
   For a given permanent value p, let A_p be the 9×9 adjacency matrix
   where A_p[s][t] = 1 iff the permanent of rows s and t equals p.
   Then a(n) = Σ_{p=0..8} 1ᵀ·A_pⁿ·1, i.e. the sum of all entries of
   A_pⁿ.  The matrices are powers‑multiplied by binary exponentiation.
   This runs in O(9³·log n) time per call. */

#include <stddef.h>

typedef struct { seq_t a[9][9]; } Matrix;

static void mat_identity(Matrix *I)
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            I->a[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

static void mat_mul(const Matrix *A, const Matrix *B, Matrix *C)
{
    seq_t temp[9][9];
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < 9; ++k)
                sum += A->a[i][k] * B->a[k][j];
            temp[i][j] = sum;
        }
    }
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            C->a[i][j] = temp[i][j];
}

/* Compute R = M^exp  (exp ≥ 0)  using binary exponentiation.
   The function protects against aliasing by using temporaries. */
static void mat_power(const Matrix *M, idx_t exp, Matrix *R)
{
    Matrix res, base;
    mat_identity(&res);
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            base.a[i][j] = M->a[i][j];

    while (exp) {
        if (exp & (idx_t)1) {
            Matrix temp;
            mat_mul(&res, &base, &temp);
            res = temp;
        }
        Matrix temp;
        mat_mul(&base, &base, &temp);
        base = temp;
        exp >>= 1;
    }
    *R = res;
}

seq_t a(idx_t n)
{
    /* The problem is defined for n ≥ 1.  We allow any n, but only
       correct for n ≥ 1. */
    if (n < 1)
        return (seq_t)0;

    /* Build adjacency matrices A_p for permanents p = 0..8. */
    Matrix A[9];
    for (int p = 0; p < 9; ++p) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                A[p].a[i][j] = (seq_t)0;
    }

    for (int s = 0; s < 9; ++s) {
        int a0 = s / 3;
        int a1 = s % 3;
        for (int t = 0; t < 9; ++t) {
            int b0 = t / 3;
            int b1 = t % 3;
            int p = a0 * b1 + a1 * b0;   /* permanent of rows s and t */
            A[p].a[s][t] = (seq_t)1;
        }
    }

    /* Count walks of length n with a fixed permanent. */
    seq_t total = (seq_t)0;
    Matrix Mp;
    for (int p = 0; p < 9; ++p) {
        mat_power(&A[p], n, &Mp);
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                total += Mp.a[i][j];
    }
    return total;
}
