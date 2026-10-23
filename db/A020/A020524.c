/*
 * A020524  a(n) = 4th Euler polynomial evaluated at 2^n.
 * https://oeis.org/A020524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^(1+3*n) + 16^n, n >= 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t p2n  = pow_uint(2, n);                  /* 2^n          */
    seq_t p1p3n = pow_uint(2, 1 + 3*n);           /* 2^(1+3n)     */
    seq_t p16n = pow_uint(16, n);                 /* 16^n         */

    return p2n - p1p3n + p16n;
}
