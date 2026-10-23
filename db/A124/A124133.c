/*
 * A124133  a(n) = (-1/2)*Sum_{i1 + i2 + i3 = 2*n} ((2*n)!/(i1! i2!
 *          i3!))*B(i1), where B are the Bernoulli numbers (with i1, i2, i3 >=
 *          1).
 * https://oeis.org/A124133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n - 1) - n - 1 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* n=0 is outside the offset; return 0 */

    /* exponent for the power of two */
    idx_t exp = 2 * n - 1;

    /* compute 2^(2*n-1) */
    seq_t val = pow2(exp);

    /* subtract n and 1 */
    val -= (seq_t)n;
    val -= 1;

    return val;
}
