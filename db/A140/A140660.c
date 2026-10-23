/*
 * A140660  a(n) = 3*4^n + 1.
 * https://oeis.org/A140660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*4^n + 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t res = pow4(n);    /* 4^n */
    res *= (seq_t)3;        /* 3*4^n */
    res += (seq_t)1;        /* +1 */
    return res;
}
