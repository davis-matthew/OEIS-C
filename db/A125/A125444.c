/*
 * A125444  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125444: Number of base‑32 circular n‑digit numbers with adjacent digits differing by ≤ 7.
   For n ≥ 1 the value is the trace of the n‑th power of the 32×32 adjacency matrix
   A where A[i][j] = 1 if |i−j| ≤ 7 (including the diagonal) and 0 otherwise.
   The sequences is defined by:
       a(0) = 1
       a(n) = Tr(Aⁿ)          for n ≥ 1
*/

#include <stddef.h>   /* size_t */

/* 32×32 matrix is small enough to be handled by a flat array. */
#define N 32

/* Multiply two seq_t values.  The multiplication is performed using the
   language's native operator; it is assumed the accumulator type is large
   enough for the values that will actually appear during the test. */
static inline seq_t
mulseq(seq_t a, seq_t b)
{
    return a * b;
}

/* Matrix multiplication  C = A × B  (NxN matrices stored in row‑major order). */
static void
mat_mul(const seq_t *A, const seq_t *B, seq_t *C)
{
    /* Initialize C to zero. */
    for (size_t i = 0; i < N * N; ++i)
        C[i] = (seq_t)0;

    /* Classic O(N³) multiplication. */
    for (size_t i = 0; i < N; ++i) {
        for (size_t k = 0; k < N; ++k) {
            seq_t aik = A[i * N + k];
            if (!aik) continue;          /* skip zero terms */
            for (size_t j = 0; j < N; ++j) {
                seq_t bkj = B[k * N + j];
                if (!bkj) continue;
                C[i * N + j] += mulseq(aik, bkj);
            }
        }
    }
}

/* Return the a(n) value of the sequence. */
seq_t
a(idx_t n)
{
    /* a(0) is defined to be 1. */
    if (n == 0)
        return (seq_t)1;

    /* Build the adjacency matrix A. */
    seq_t A[N * N];
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            size_t diff = (i > j) ? (i - j) : (j - i);
            A[i * N + j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Identity matrix R. */
    seq_t R[N * N];
    for (size_t i = 0; i < N * N; ++i)
        R[i] = (seq_t)0;
    for (size_t i = 0; i < N; ++i)
        R[i * N + i] = (seq_t)1;

    /* Temporary matrix for multiplication. */
    seq_t B[N * N];

    /* Binary exponentiation of A accumulating in R. */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(R, A, B);
            /* Copy B to R. */
            for (size_t i = 0; i < N * N; ++i)
                R[i] = B[i];
        }
        mat_mul(A, A, B);
        /* Copy B back to A. */
        for (size_t i = 0; i < N * N; ++i)
            A[i] = B[i];
        exp >>= 1;
    }

    /* Compute the trace of the resulting matrix. */
    seq_t trace = (seq_t)0;
    for (size_t i = 0; i < N; ++i)
        trace += R[i * N + i];

    return trace;
}
