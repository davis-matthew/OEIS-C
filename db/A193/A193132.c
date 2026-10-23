/*
 * A193132  a(n) = 3n*4^(2n-1).
 * https://oeis.org/A193132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*n*4^(2*n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute exponent: 2*n - 1 */
    idx_t exp = (idx_t)(2 * n - 1);
    /* Calculate 4^(2*n-1) */
    seq_t pow4 = pow_uint(4, exp);
    /* Multiply by n and 3 */
    seq_t result = pow4;
    result *= (seq_t)n;
    result *= (seq_t)3;
    return result;
}
