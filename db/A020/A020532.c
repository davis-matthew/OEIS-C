/*
 * A020532  a(n) = 6th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (3 + 4^(1+n) + 16^n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow2n  = pow_uint(2, n);       /* 2^n */
    seq_t pow4n1 = pow_uint(4, n + 1);   /* 4^(1+n) */
    seq_t pow16n = pow_uint(16, n);      /* 16^n */

    seq_t sum = 3;                       /* start with constant 3 */
    sum += pow4n1;                       /* add 4^(1+n) */
    sum += pow16n;                       /* add 16^n */

    return pow2n * sum;                  /* final product */
}
