/*
 * A124705  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Trace(A^n) where A is the 12×12 adjacency matrix of the
 *            graph with vertices 0…11 and edges between vertices whose labels
 *            differ by at most 1 (including loops). That is, a(n) counts the
 *            base‑12 circular n‑digit numbers with adjacent digits differing
 *            by ≤1. The function computes A^n by fast...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* only standard headers that are safe for C11/C++17 */
#include <stddef.h>
#include <stdint.h>

/* 12×12 matrix of seq_t */
#define K 12

/* matrix multiplication: C = A * B  (in place, dest is separate) */
static void mat_mul(seq_t C[K][K], const seq_t A[K][K], const seq_t B[K][K])
{
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < K; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* trace of a square matrix */
static seq_t mat_trace(const seq_t M[K][K])
{
    seq_t t = (seq_t)0;
    for (int i = 0; i < K; ++i) {
        t += M[i][i];
    }
    return t;
}

/* Sequence A124705: number of base‑12 circular n‑digit numbers
   with adjacent digits differing by 1 or less. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;          /* a(0) = 1 by definition */
    }

    /* adjacency matrix A */
    seq_t base[K][K];
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            int diff = i > j ? i - j : j - i;   /* absolute difference */
            base[i][j] = (diff <= 1) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* identity matrix for exponentiation */
    seq_t res[K][K] = {{0}};
    for (int i = 0; i < K; ++i) {
        res[i][i] = (seq_t)1;
    }

    idx_t e = n;
    seq_t tmp[K][K];
    /* fast exponentiation of base to the power n */
    while (e > 0) {
        if (e & 1) {
            mat_mul(tmp, res, base);   /* tmp = res * base */
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(tmp, base, base);       /* tmp = base * base */
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                base[i][j] = tmp[i][j];
        e >>= 1;
    }

    return mat_trace(res);
}
