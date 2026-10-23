/*
 * A028089  Expansion of 1/((1-3x)(1-7x)(1-8x)(1-9x)).
 * https://oeis.org/A028089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*9^(n+3)-24*8^(n+3)+15*7^(n+3)-3^(n+3))/120
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t e = n + 3;
    seq_t t9 = pow_uint(9, e);
    seq_t t8 = pow_uint(8, e);
    seq_t t7 = pow_uint(7, e);
    seq_t t3 = pow_uint(3, e);

    seq_t res = t9 * 10;
    res -= t8 * 24;
    res += t7 * 15;
    res -= t3;
    res /= 120;
    return res;
}
