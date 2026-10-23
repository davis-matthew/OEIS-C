/*
 * A014236  Expansion of g.f.: 2*x*(1-x)/((1-2*x)*(1-2*x^2)).
 * https://oeis.org/A014236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^n if n is odd,
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = 2^(n/2) * (2^(n/2) - 1) if n is even. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n & 1)                /* odd n */
    {
        return pow2(n);
    }
    else                     /* even n */
    {
        seq_t h = pow2(n / 2);
        return h * (h - 1);
    }
}
