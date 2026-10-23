/*
 * A181174  The "Row2" sums of the powers-of-2 triangle A000079.
 * https://oeis.org/A181174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n*(n+1)/2) * (2^(n+1)+(-1)^n)/3
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* exponent for the first power of 2 */
    idx_t tri = (idx_t)n * ((idx_t)n + 1) / 2;

    seq_t part1 = pow2(tri);           /* 2^(n*(n+1)/2) */
    seq_t part2 = pow2((idx_t)n + 1);   /* 2^(n+1) */

    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;   /* (-1)^n */
    seq_t divisor = (part2 + sign) / 3;              /* (2^(n+1)+(-1)^n)/3 */

    return part1 * divisor;
}
