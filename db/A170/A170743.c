/*
 * A170743  Expansion of g.f.: (1+x)/(1-23*x).
 * https://oeis.org/A170743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = 24*23^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* compute 23^(n-1) and multiply by 24 */
    seq_t pw = pow_uint(23, n - 1);
    return (seq_t)24 * pw;
}
