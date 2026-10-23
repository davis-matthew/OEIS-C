/*
 * A027794  a(n) = 12*(n+1)*binomial(n+3,9).
 * https://oeis.org/A027794
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*(n+1)*C(n+3,9)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 26, u64 n <= 237, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(n+3,9) */
    seq_t bin = binom((idx_t)(n + 3), (idx_t)9);

    /* Compute the multiplier 12*(n+1) using seq_t arithmetic */
    seq_t mul = (seq_t)12 * (seq_t)(n + 1);

    /* Return the product */
    return mul * bin;
}
