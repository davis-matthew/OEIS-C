/*
 * A215713  Denominator of sum(i=1..n, 3*i/4^i).
 * https://oeis.org/A215713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 2^(2n - v2(3n + 4)) (v2 = exponent of 2 dividing a
 *            number)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute v2(3n + 4) */
    idx_t t = 3 * n + 4;
    idx_t v2 = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        ++v2;
    }

    /* Exponent of the denominator: 2n - v2(3n + 4) */
    idx_t exp = 2 * n - v2;

    return pow2(exp);
}
