/*
 * A124715  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of base‑22 circular n‑digit numbers whose adjacent
 * digits differ by at most 1.
 *
 * Formula (by transfer‑matrix method):
 *   a(n) = trace( A^n )
 *   where A is the 22×22 adjacency matrix of a path graph with self‑loops
 *   (A[i][j] = 1  if |i−j| ≤ 1, 0 otherwise).
 * This is equivalent to a(n) = ∑_{j=1}^{22} (1+2·cos(jπ/23))^n.
 * The function works for any seq_t/idx_t type, as long as the usual
 * arithmetic operators are available and seq_t values are copied
 * by assignment, not by shallow memcpy. */

#include <stdint.h>

#define K 22

seq_t a(idx_t n)
{
    /* a(0) = 1 (empty word) */
    if (n == 0)
        return 1;

    /* Build the base adjacency matrix A. */
    seq_t A[K][K];
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = 0; j < K; ++j) {
            if ((i == j) || (i + 1 == j) || (i + 1 == j) || (i + 1 == j))  /* placeholder to keep too long if we skip this line*/ {}
        }
    }
    /* Actually set entries properly */
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = 0; j < K; ++j) {
            if (i == j || i + 1 == j || i == j + 1)
                A[i][j] = 1;
            else
                A[i][j] = 0;
        }
    }

    /* R = identity matrix */
    seq_t R[K][K];
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = 0; j < K; ++j) {
            R[i][j] = (i == j) ? 1 : 0;
        }
    }

    /* B = A */
    seq_t B[K][K];
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = 0; j < K; ++j) {
            B[i][j] = A[i][j];
        }
    }

    /* Temporary matrix to hold intermediate results */
    seq_t T[K][K];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* T = R * B */
            for (idx_t i = 0; i < K; ++i) {
                for (idx_t j = 0; j < K; ++j) {
                    seq_t sum = 0;
                    for (idx_t k = 0; k < K; ++k) {
                        sum += R[i][k] * B[k][j];
                    }
                    T[i][j] = sum;
                }
            }
            /* R = T */
            for (idx_t i = 0; i < K; ++i)
                for (idx_t j = 0; j < K; ++j)
                    R[i][j] = T[i][j];
        }

        /* T = B * B */
        for (idx_t i = 0; i < K; ++i) {
            for (idx_t j = 0; j < K; ++j) {
                seq_t sum = 0;
                for (idx_t k = 0; k < K; ++k) {
                    sum += B[i][k] * B[k][j];
                }
                T[i][j] = sum;
            }
        }

        /* B = T */
        for (idx_t i = 0; i < K; ++i)
            for (idx_t j = 0; j < K; ++j)
                B[i][j] = T[i][j];

        exp >>= 1;
    }

    /* The answer is the trace of A^n (sum of diagonal entries of R). */
    seq_t ans = 0;
    for (idx_t i = 0; i < K; ++i)
        ans += R[i][i];

    return ans;
}
