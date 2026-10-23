/*
 * A122746  Expansion of o.g.f. 1/((1-2*x)*(1-2*x^2)).
 * https://oeis.org/A122746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 2^floor((n+1)/2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t e1 = n + 1;       /* exponent n+1 */
    idx_t e2 = (n + 1) >> 1;/* exponent floor((n+1)/2) */
    return pow2(e1) - pow2(e2);
}
