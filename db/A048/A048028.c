/*
 * A048028  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(m, n/3, (n-m)/2)} binom(m, k) * binom(n-m,
 *            2k), where m = ⌊(n-3)/3⌋. Count nonempty subsets of {1,…,n} with
 *            exactly 1/3 of elements ≤ m
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute m = floor((n-3)/3), handling n < 3 */
    idx_t m = (n >= 3) ? (n - 3) / 3 : 0;

    /* Upper bounds for k:   k ≤ m   (cannot choose more low elements than exist)
                                 k ≤ n/3   (size 3k must not exceed n)
                                 k ≤ (n-m)/2  (cannot choose more high elements)
    */
    idx_t kmax1 = m;
    idx_t kmax2 = n / 3;
    idx_t kmax3 = (n >= m) ? (n - m) / 2 : 0;
    idx_t kmax = kmax1;
    if (kmax2 < kmax) kmax = kmax2;
    if (kmax3 < kmax) kmax = kmax3;

    if (kmax == 0)
        return 0;

    seq_t total = 0;
    for (idx_t k = 1; k <= kmax; ++k)
    {
        seq_t low  = binom(m, k);               /* choose k low elements */
        seq_t high = binom(n - m, 2 * k);       /* choose 2k high elements */
        total += low * high;
    }
    return total;
}
