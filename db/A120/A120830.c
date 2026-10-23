/*
 * A120830  a(n) = 5^(n^2)-3^n.
 * https://oeis.org/A120830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n^2) - 3^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t exp = n * n;                 /* exponent for 5: n^2 */
    seq_t p5 = pow_uint(5, exp);       /* 5^(n^2) */
    seq_t p3 = pow_uint(3, n);         /* 3^n      */
    return p5 - p3;                    /* difference */
}
