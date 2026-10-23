/*
 * A034660  Sum of n-th powers of divisors of 12.
 * https://oeis.org/A034660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 3^n + 4^n + 6^n + 12^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* sum of n-th powers of the six divisors of 12 */
    seq_t res = 1;               /* 1^n is always 1 */
    res += pow_uint(2, n);       /* 2^n */
    res += pow_uint(3, n);       /* 3^n */
    res += pow_uint(4, n);       /* 4^n */
    res += pow_uint(6, n);       /* 6^n */
    res += pow_uint(12, n);      /* 12^n */
    return res;
}
