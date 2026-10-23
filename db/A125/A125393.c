/*
 * A125393  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125393
   a(0) = 1
   For n >= 1, a(n) = trace(A^n) where A is the 30×30 adjacency matrix
   A[i][j] = 1 if |i - j| <= 5, otherwise 0.  The matrix is symmetric and
   the trace counts the number of length‑n circular base‑30 strings with
   adjacent digits differing by at most 5.  */
#include <stdint.h>

#define N 30   /* size of the adjacency matrix */

/* adjacency matrix */
static seq_t A[N][N];
static int A_initialized = 0;

/* initialise A only once */
static void init_adj(void)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int d = i - j;
            if (d < 0) d = -d;
            A[i][j] = (d <= 5) ? (seq_t)1 : (seq_t)0;
        }
    }
    A_initialized = 1;
}

/* matrix multiplication: R = X * Y  (all N×N) */
static void mat_mul(seq_t R[N][N], const seq_t X[N][N], const seq_t Y[N][N])
{
    /* initialise result to zero */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            R[i][j] = (seq_t)0;

    /* naive multiplication */
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            seq_t aik = X[i][k];
            if (aik == (seq_t)0) continue;
            for (int j = 0; j < N; ++j) {
                R[i][j] += aik * Y[k][j];
            }
        }
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    if (!A_initialized) init_adj();

    /* matrices used in exponentiation */
    static seq_t M[N][N];
    static seq_t res[N][N];
    static seq_t tmp[N][N];

    /* copy adjacency matrix into M */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            M[i][j] = A[i][j];

    /* identity matrix into res */
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(tmp, res, M);
            /* copy tmp back to res */
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    res[i][j] = tmp[i][j];
        }
        /* square the matrix M */
        mat_mul(tmp, M, M);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                M[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* compute trace of res = A^n */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < N; ++i)
        trace += res[i][i];

    return trace;
}
