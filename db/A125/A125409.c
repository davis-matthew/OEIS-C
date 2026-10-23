/*
 * A125409  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = trace(Aⁿ), where A[i][j] = 1 if |i −
 *            j| ≤ 6, and 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n‑th term of OEIS A125409. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Adjacency matrix for the 21‑digit alphabet. */
    static seq_t A[21][21];
    static bool adj_init = false;
    if (!adj_init) {
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 21; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                A[i][j] = (diff <= 6) ? 1 : 0;
            }
        }
        adj_init = true;
    }

    /* Matrix exponentiation: res = Aⁿ. */
    static seq_t res[21][21];
    static seq_t power[21][21];

    /* Initialise res = I, power = A. */
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            res[i][j]   = (i == j) ? 1 : 0;
            power[i][j] = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1ULL) {
            /* res = res * power */
            seq_t tmp[21][21];
            for (int i = 0; i < 21; ++i) {
                for (int j = 0; j < 21; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 21; ++k)
                        sum = sum + res[i][k] * power[k][j];
                    tmp[i][j] = sum;
                }
            }
            for (int i = 0; i < 21; ++i)
                for (int j = 0; j < 21; ++j)
                    res[i][j] = tmp[i][j];
        }

        /* power = power * power */
        seq_t tmpPow[21][21];
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 21; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 21; ++k)
                    sum = sum + power[i][k] * power[k][j];
                tmpPow[i][j] = sum;
            }
        }
        for (int i = 0; i < 21; ++i)
            for (int j = 0; j < 21; ++j)
                power[i][j] = tmpPow[i][j];

        exp >>= 1ULL;
    }

    /* Sum the diagonal entries to obtain the trace. */
    seq_t total = 0;
    for (int i = 0; i < 21; ++i)
        total = total + res[i][i];

    return total;
}
