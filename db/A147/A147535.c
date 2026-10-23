/*
 * A147535  a(n) = 2*4^n+4*3^n-2^n.
 * https://oeis.org/A147535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*4^n + 4*3^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p2 = pow_uint(2, n);   /* 2^n   */
    seq_t p3 = pow_uint(3, n);   /* 3^n   */
    seq_t p4 = pow_uint(4, n);   /* 4^n   */

    seq_t res = 2 * p4 + 4 * p3 - p2;
    return res;
}
