/*
 * A218499  7th iteration of the hyperbinomial transform on the sequence of
 *          1's.
 * https://oeis.org/A218499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} 7 * (n-j+7)^(n-j-1) * binomial(n,j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

/* Computes A218499: the 7th iteration of the hyperbinomial transform on the
 * sequence of 1's. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t j = 0; j <= n; ++j) {
        idx_t m = n - j;          /* m = n-j */
        seq_t term;
        if (m == 0) {
            /* When m==0 we have 7*7^(-1) = 1. */
            term = 1;
        } else {
            /* (m+7)^(m-1) */
            seq_t pow_term = seq_pow((seq_t)(m + 7), m - 1);

            /* binomial(n, j) computed multiplicatively */
            seq_t bin = 1;
            for (idx_t k = 1; k <= j; ++k) {
                bin = bin * (n - k + 1) / k;
            }

            term = pow_term * bin * (seq_t)7;
        }
        result += term;
    }
    return result;
}
