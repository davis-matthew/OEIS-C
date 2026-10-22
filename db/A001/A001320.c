/*
 * A001320  Number of self-complementary Boolean functions of n variables, up
 *          to equivalence under the group (C_2)^n of all 2^n complementations
 *          of variables.
 * https://oeis.org/A001320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(2^(n-1) - n) * (2^n - 1) [= 2^(2^(n-1)) * (2^n-1) /
 *            2^n]
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   a001320_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001320_pow2.h"

seq_t a(idx_t n)
{
    idx_t half, ex;
    seq_t factor;
    if (n < 1)
        return 0;
    /* 2^(n-1) as an index-sized exponent (only meaningful for n <= 64) */
    half = (n - 1 < 64) ? ((idx_t)1 << (n - 1)) : 0;
    ex = half - n;
    factor = a001320_pow2(n) - 1;
    return a001320_pow2(ex) * factor;
}
