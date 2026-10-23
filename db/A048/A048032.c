/*
 * A048032  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(L, G/4, n/5)} C(L, k) * C(G, 4k) * where L =
 *            floor((n-3)/3), G = n - L * (C is the binomial coefficient) *
 *            This counts nonempty subsets of {1,…,n} in which exactly 1/5 of
 *            the * elements are ≤ (n-3)/3
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Count of elements ≤ (n-3)/3, using unsigned arithmetic */
    idx_t L = (n < 3) ? 0 : (n - 3) / 3;
    idx_t G = n - L;

    /* No valid subset if there are no eligible small elements or
     * not enough large elements to fill ≥ 4k elements. */
    if (L == 0 || G < 4)
        return 0;

    /* Maximum possible k satisfying all constraints */
    idx_t tmax = L;
    if (G / 4 < tmax)
        tmax = G / 4;
    if (n / 5 < tmax)
        tmax = n / 5;

    seq_t sum = 0;
    for (idx_t t = 1; t <= tmax; ++t)
    {
        seq_t part1 = binomial(L, t);
        seq_t part2 = binomial(G, 4 * t);
        sum += part1 * part2;
    }
    return sum;
}
