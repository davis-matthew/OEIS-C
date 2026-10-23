/*
 * A048047  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{⌊min(n/6, (n-m)/5)⌋} C(m, t) * C(n-m, 5*t) where
 *            m = ⌊(n+1)/3⌋. Count nonempty subsets of {1,…,n} in which
 *            exactly 1/6 of the elements are ≤ (n+1)/3
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* threshold m = floor((n+1)/3) */
    idx_t m = (n + 1) / 3;

    /* maximum possible t */
    idx_t t_max = n / 6;
    idx_t tmp = (n - m) / 5;
    if (tmp < t_max) t_max = tmp;

    seq_t total = 0;
    for (idx_t t = 1; t <= t_max; ++t) {
        seq_t part1 = binomial(m, t);
        seq_t part2 = binomial(n - m, 5 * t);
        total += part1 * part2;
    }
    return total;
}
