/*
 * A048036  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{min(⌊(n-3)/3⌋, ⌊(n-⌊(n-3)/3⌋)/5⌋)} C(⌊(n-3)/3⌋,
 *            t) · C(n-⌊(n-3)/3⌋, 5t)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(a,b) is the binomial coefficient. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 6) return (seq_t)0;

    idx_t s = (n - 3) / 3;          /* floor((n-3)/3)          */
    idx_t l = n - s;                /* elements > s            */
    idx_t max_t = s;
    idx_t l_div5 = l / 5;
    if (l_div5 < max_t) max_t = l_div5;

    seq_t sum = 0;
    for (idx_t t = 1; t <= max_t; ++t)
    {
        seq_t comb_small = binomial(s, t);          /* C(s, t) */
        seq_t comb_large = binomial(l, 5 * t);      /* C(l, 5t) */
        sum += comb_small * comb_large;
    }
    return sum;
}
