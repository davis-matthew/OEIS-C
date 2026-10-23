/*
 * A047180  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1} binomial(floor((n-1)/2), t) * binomial(n -
 *            floor((n-1)/2), 5t) where t runs while 5t ≤ n - floor((n-1)/2)
 *            and t ≤ floor((n-1)/2). This counts the non‑empty subsets of
 *            {1,…,n} that contain exactly 1/6 of the elements from the lower
 *            half
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* size of the lower half: floor((n-1)/2) */
    idx_t m = (n - 1) / 2;
    /* size of the upper half: n - m = ceil((n+1)/2) */
    idx_t upper = n - m;

    /* Maximum t such that 5t ≤ upper and t ≤ m */
    idx_t tmax = upper / 5;
    if (tmax > m) tmax = m;

    seq_t sum = 0;
    for (idx_t t = 1; t <= tmax; ++t)
    {
        seq_t comb1 = binomial(m, t);
        seq_t comb2 = binomial(upper, 5 * t);
        sum += comb1 * comb2;
    }
    return sum;
}
