/*
 * A020714  a(n) = 5 * 2^n.
 * https://oeis.org/A020714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * 2^n
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n as seq_t */
    seq_t res = pow2(n);
    /* multiply by 5 */
    res *= (seq_t)5;
    return res;
}
