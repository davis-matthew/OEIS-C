/*
 * A022918  Multinomial coefficients(TOP, BOTTOM), where TOP = n(n+1)(2n+1)/6,
 *          BOTTOM = ( 1^2 2^2 ... n^2 ).
 * https://oeis.org/A022918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum_{i=1}^n i^2)! / prod_{i=1}^n (i^2)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined; the sequence is 1-indexed. */
    if (n == 0)
        return 0;

    seq_t result = 1;          /* running product of binomial coefficients */
    idx_t sum = 0;              /* current value of sum_{i=1}^k i^2 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t k2 = (idx_t)k * k;   /* k^2, fits in 64‑bit for all n up to 10000 */
        sum += k2;                 /* update the sum of squares */

        seq_t bin = binom(sum, k2);/* C(sum, k^2) */
        result = result * bin;     /* multiply the running product */
    }

    return result;
}
