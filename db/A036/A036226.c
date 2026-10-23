/*
 * A036226  Expansion of 1/(1-7*x)^7.
 * https://oeis.org/A036226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n * binomial(n+6, 6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 7^n modulo the width of seq_t */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)7;

    /* Compute the binomial coefficient C(n+6, 6) */
    seq_t bin = binomial(n + 6, 6);

    return pow * bin;
}
