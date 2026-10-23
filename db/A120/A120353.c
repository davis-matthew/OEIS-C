/*
 * A120353  Sum of 5 consecutive powers of 3, starting with a power of 9.
 * https://oeis.org/A120353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121 * 9^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0
       as a safe sentinel value; behaviour for n < 1 is not used. */
    if (n == 0)
        return (seq_t)0;
    seq_t val = pow_uint(9u, n - 1);   /* 9^(n-1) */
    val *= (seq_t)121;                 /* 121 * 9^(n-1) */
    return val;
}
