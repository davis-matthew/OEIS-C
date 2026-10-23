/*
 * A125400  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 12×12 adjacency matrix * with
 *            A[i][j] = 1 if |i - j| ≤ 6, otherwise 0. a(0) = 1. This * counts
 *            the number of circular base‑12 n‑digit numbers whose * adjacent
 *            digits differ by at most 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* 12×12 adjacency matrix for the graph.  It is constant after first
       use, so compute once in a static context. */
    static seq_t base[12][12];
    static bool initialized = false;
    if (!initialized) {
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                int d = (i >= j) ? (i - j) : (j - i);
                base[i][j] = (seq_t)(d <= 6 ? 1 : 0);
            }
        }
        initialized = true;
    }

    /* Helper matrices: result, current power, and temporary storage. */
    seq_t res[12][12];
    seq_t cur[12][12];
    seq_t tmp[12][12];

    /* Initialize result to the identity matrix and cur to the base matrix. */
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            res[i][j]  = (i == j) ? (seq_t)1 : (seq_t)0;
            cur[i][j]  = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* res = res * cur */
            for (int i = 0; i < 12; ++i) {
                for (int j = 0; j < 12; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 12; ++k) {
                        sum += res[i][k] * cur[k][j];
                    }
                    tmp[i][j] = sum;
                }
            }
            for (int i = 0; i < 12; ++i) {
                for (int j = 0; j < 12; ++j) {
                    res[i][j] = tmp[i][j];
                }
            }
        }

        /* cur = cur * cur (square of the current matrix) */
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 12; ++k) {
                    sum += cur[i][k] * cur[k][j];
                }
                tmp[i][j] = sum;
            }
        }
        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                cur[i][j] = tmp[i][j];
            }
        }

        exp >>= 1;
    }

    /* Compute the trace of res. */
    seq_t trace = 0;
    for (int i = 0; i < 12; ++i) {
        trace += res[i][i];
    }
    return trace;
}
