/*
 * A048040  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{min(⌊(n+1)/3⌋/2, n-⌊(n+1)/3⌋)} binom(⌊(n+1)/3⌋,
 *            2t) * binom(n-⌊(n+1)/3⌋, t)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = (n + 1) / 3;      /* number of elements ≤ (n+1)/3 */
    idx_t r = n - m;            /* number of elements > (n+1)/3 */
    idx_t max_t = m / 2;
    if (max_t > r) max_t = r;

    seq_t sum = 0;
    for (idx_t t = 1; t <= max_t; ++t) {
        seq_t small = binom(m, 2 * t);
        seq_t big   = binom(r, t);
        sum += small * big;
    }
    return sum;
}
