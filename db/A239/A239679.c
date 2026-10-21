/*
 * A239679  Least number k > 0 such that k*2^n+1 is a cube.
 * https://oeis.org/A239679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 3*2^n + 2^(2*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p  = pow2(n);          /* 2^n   */
    seq_t p2 = pow2(n << 1);    /* 2^(2n) */
    return 3 + 3 * p + p2;
}
