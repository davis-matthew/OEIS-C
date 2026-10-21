/*
 * A075184  One half of fourth column of triangle A075181.
 * https://oeis.org/A075184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)! * (6*C(n+4,4) + 20*C(n+4,5) + 15*C(n+4,6))) / 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   _binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_binom_small.h"

seq_t a(idx_t n)
{
    /* m = n + 4 */
    idx_t m = n + 4;

    /* Compute factorial (n+1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fact *= (seq_t)i;

    /* Compute the three binomial terms */
    seq_t c4 = _binom_small(m, 4);
    seq_t c5 = _binom_small(m, 5);
    seq_t c6 = _binom_small(m, 6);

    /* Sum with integer coefficients */
    seq_t sum = 6 * c4 + 20 * c5 + 15 * c6;

    /* Multiply by factorial and divide by 2 */
    seq_t result = fact * sum / 2;

    return result;
}
