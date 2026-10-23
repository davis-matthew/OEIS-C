/*
 * A182305  a(n+1) = a(n) + floor(a(n)/4) with a(0)=4.
 * https://oeis.org/A182305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n) + floor(a(n)/4), a(0) = 4
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t val = 4;
    for (idx_t i = 0; i < n; ++i)
    {
        val += val / 4;
    }
    return val;
}
