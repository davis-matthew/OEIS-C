/*
 * A218497  5th iteration of the hyperbinomial transform on the sequence of
 *          1's.
 * https://oeis.org/A218497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j = 0..n} 5 * (n - j + 5)^(n - j - 1) * C(n, j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;          /* C(n, 0) */
    for (idx_t j = 0; j <= n; ++j) {
        idx_t m = n - j;    /* m = n - j */
        seq_t term;
        if (m == 0) {
            /* For m = 0 we have 5 * 5^-1 * C(n, n) = 1 */
            term = 1;
        } else {
            seq_t base = (seq_t)(m + 5);
            seq_t pow_val = seq_pow(base, m - 1);
            term = bin * pow_val;
            term = term * (seq_t)5;
        }
        sum = sum + term;

        /* Update the binomial coefficient for the next j                */
        if (j < n) {
            /* C(n, j+1) = C(n, j) * (n - j) / (j + 1)                   */
            seq_t num = (seq_t)(n - j);
            seq_t den = (seq_t)(j + 1);
            bin = bin * num;
            bin = bin / den;
        }
    }
    return sum;
}
