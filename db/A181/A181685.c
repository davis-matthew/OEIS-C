/*
 * A181685  Expansion of 36*x^2*(1+6*x-36*x^2) / ( (1-6*x)^2 *(1+6*x+36*x^2)
 *          ).
 * https://oeis.org/A181685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1 - 2*floor((n-1)/3))*6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1, but safe guard */

    idx_t tmp   = n - 1;                 /* n-1 */
    idx_t quo   = tmp / 3;               /* floor((n-1)/3) */
    idx_t factor = tmp - 2 * quo;        /* (n-1) - 2*floor((n-1)/3) */

    seq_t pow6 = pow_uint(6, n);         /* compute 6^n (seq_t) */
    return (seq_t)factor * pow6;         /* final result */
}
