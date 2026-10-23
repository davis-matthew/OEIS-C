/*
 * A048009  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(n-m, floor(m/3))} binom(m, 3k) *
 *            binom(n-m, k) where m = floor((n-1)/3) and binom is the standard
 *            binomial coefficient. This counts the nonempty subsets of
 *            {1,…,n} in which exactly 3/4 of the elements are <= (n-1)/3
 * Range:     u8 n <= 18, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute the threshold m = floor((n-1)/3).  */
    idx_t m = (n - 1) / 3;

    /* The number of elements greater than m.  */
    idx_t high = n - m;

    /* The largest k such that 3k <= m (i.e., k <= floor(m/3)).  */
    idx_t maxk = m / 3;

    /* Upper bound for k: also cannot exceed the number of high elements.  */
    if (high < maxk)
        maxk = high;

    if (maxk == 0)   /* no nonempty subset satisfies the condition */
        return 0;

    seq_t result = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t low_part  = binom(m, 3 * k);
        seq_t high_part = binom(high, k);
        result += low_part * high_part;
    }
    return result;
}
