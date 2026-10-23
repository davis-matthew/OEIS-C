/*
 * A124708  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = trace(A^n), where A is the 15×15
 *            adjacency matrix of the graph with vertices 0..14 and edges for
 *            |i–j| ≤ 1. This counts the number of closed walks of length n,
 *            i.e. circular n‑digit numbers in base 15 with adjacent digits
 *            differ by at most 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build the 15×15 adjacency matrix G. */
    seq_t G[15][15];
    for (idx_t i = 0; i < 15; ++i) {
        for (idx_t j = 0; j < 15; ++j) {
            int diff = (i > j) ? (int)(i - j) : (int)(j - i);
            G[i][j] = (diff <= 1) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* result := I   (identity matrix)   */
    seq_t result[15][15];
    for (idx_t i = 0; i < 15; ++i) {
        for (idx_t j = 0; j < 15; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* base := G   */
    seq_t base[15][15];
    for (idx_t i = 0; i < 15; ++i) {
        for (idx_t j = 0; j < 15; ++j) {
            base[i][j] = G[i][j];
        }
    }

    /* Temporary matrix for intermediate computations. */
    seq_t temp[15][15];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* result = result * base */
            for (idx_t i = 0; i < 15; ++i) {
                for (idx_t j = 0; j < 15; ++j) {
                    seq_t sum = (seq_t)0;
                    for (idx_t k = 0; k < 15; ++k) {
                        sum += result[i][k] * base[k][j];
                    }
                    temp[i][j] = sum;
                }
            }
            for (idx_t i = 0; i < 15; ++i)
                for (idx_t j = 0; j < 15; ++j)
                    result[i][j] = temp[i][j];
        }

        /* base = base * base */
        for (idx_t i = 0; i < 15; ++i) {
            for (idx_t j = 0; j < 15; ++j) {
                seq_t sum = (seq_t)0;
                for (idx_t k = 0; k < 15; ++k) {
                    sum += base[i][k] * base[k][j];
                }
                temp[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < 15; ++i)
            for (idx_t j = 0; j < 15; ++j)
                base[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* Compute the trace of the resulting matrix. */
    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < 15; ++i) {
        trace += result[i][i];
    }
    return trace;
}
