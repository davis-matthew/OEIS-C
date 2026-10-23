/*
 * A132477  Row sums of triangle A132476.
 * https://oeis.org/A132477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(n)=3*2^n (n>1)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    seq_t val = pow2(n);      /* 2^n */
    val *= (seq_t)3;          /* 3 * 2^n */
    return val;
}
