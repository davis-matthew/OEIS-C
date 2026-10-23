/*
 * A155664  a(n) = 10^n+8^n-1.
 * https://oeis.org/A155664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n+8^n-1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p10 = pow_uint(10, n);
    seq_t p8  = pow_uint(8, n);
    return p10 + p8 - 1;
}
