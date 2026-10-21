/*
 * A204544  Fractional part of (3/2)^n without the decimal point.
 * https://oeis.org/A204544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = fractional part of (3/2)^n without decimal point. For any
 *            n ≥ 0: a(n) = ( (3^n mod 2^n) * 5^n )
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t pow2 = pow_uint(2, n);   /* 2^n   */
    seq_t pow3 = pow_uint(3, n);   /* 3^n   */
    seq_t r    = pow3 % pow2;      /* (3^n mod 2^n) */
    seq_t pow5 = pow_uint(5, n);   /* 5^n   */
    return r * pow5;               /* r * 5^n */
}
