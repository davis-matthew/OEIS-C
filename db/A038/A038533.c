/*
 * A038533  Denominator of coefficients of both EllipticK/Pi and EllipticE/Pi.
 * https://oeis.org/A038533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(1+4*n-2*w(n)), where w(n) is the popcount of n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t w = (idx_t)__builtin_popcountll((unsigned long long)n);
    idx_t e = 1 + 4 * n - 2 * w;
    return pow2(e);
}
