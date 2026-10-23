/*
 * A047176  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{min(⌊(n−1)/2⌋, ⌊(n−⌊(n−1)/2⌋)/4⌋)} C(⌊(n−1)/2⌋,
 *            a) · C(n−⌊(n−1)/2⌋, 4·a)
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The subset must contain exactly 1/5 of its elements from the lower half
 * of {1,…,n}.  This implies that |S| = 5·a with a≤⌊(n−1)/2⌋ and that
 * the upper half contains 4·a elements, hence 4·a ≤ n−⌊(n−1)/2⌋.
 * The program counts all such subsets using incremental binomial
 * coefficients. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 5)               /* no subset can have size ≥ 5 */
        return 0;

    /* lower half size (≤ (n−1)/2) and upper half size */
    idx_t m = (n - 1) / 2;
    idx_t ur = n - m;

    /* maximal a satisfying a ≤ m and 4a ≤ ur */
    idx_t a_max = (ur / 4 < m) ? ur / 4 : m;
    if (a_max == 0)
        return 0;

    seq_t result = 0;

    /* binomial coefficient C(m, a), starting with a = 0 -> 1  */
    seq_t comb_low = 1;

    /* binomial coefficient C(ur, k), where k = 4a.
       Start with k = 0 -> 1, and increase k by 4 each iteration. */
    seq_t comb_up   = 1;
    idx_t k         = 0;           /* current k = 4·(current a − 1) */

    for (idx_t a = 1; a <= a_max; ++a)
    {
        /* Update C(m, a) from C(m, a−1) */
        comb_low = comb_low * (m - a + 1) / a;

        /* Update C(ur, k+4) from C(ur, k) */
        for (idx_t t = 1; t <= 4; ++t)
        {
            comb_up = comb_up
                      * (ur - (k + t - 1))       /* numerator */
                      / (k + t);                /* denominator */
        }
        k += 4;

        /* Add contribution for this a */
        result += comb_low * comb_up;
    }

    return result;
}
