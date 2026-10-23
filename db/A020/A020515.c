/*
 * A020515  a(n) = 4^n - 2^n + 1.
 * https://oeis.org/A020515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^n + 1, offset 0 (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    idx_t two_n = n << 1;          /* 2*n */
    seq_t p4 = pow2(two_n);       /* 4^n */
    seq_t p2 = pow2(n);           /* 2^n */
    return p4 - p2 + 1;           /* 4^n - 2^n + 1 */
}
