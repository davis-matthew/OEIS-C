/*
 * A116970  a(n) = (3^n - 7)/2.
 * https://oeis.org/A116970
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 7)/2 for n >= 2, with a(2) = 1. The * sequence is
 *            defined for all n >= 2. The implementation uses * pow_uint from
 *            the helpers to compute 3^n exactly (in the * precision of seq_t)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For indices below the offset, return 0 (undefined). */
    if (n < 2)
        return 0;

    seq_t p = pow_uint(3, n);   /* compute 3^n   */
    p -= 7;                     /* subtract 7    */
    p /= 2;                     /* divide by 2   */
    return p;
}
