/*
 * A159274  a(0)=209; for n > 0, a(n) = a(n-1) + floor(sqrt(a(n-1))).
 * https://oeis.org/A159274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=209; for n>0, a(n)=a(n-1)+floor(sqrt(a(n-1)))
 * Range:     u8 n <= 0, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = 209;        /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        val += floor_sqrt(val);
    }
    return val;
}
