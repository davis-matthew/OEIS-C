/*
 * A141722  a(n) = 8*4^n - 7.
 * https://oeis.org/A141722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*4^n - 7
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t val = pow_uint(4, n); /* compute 4^n */
    val *= (seq_t)8;            /* multiply by 8 */
    val -= (seq_t)7;            /* subtract 7 */
    return val;
}
