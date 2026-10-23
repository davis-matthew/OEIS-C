/*
 * A048070  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n-4)/2.
 * https://oeis.org/A048070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m≥1} C(⌊(n-4)/2⌋, 5m) * C(n-⌊(n-4)/2⌋, m) with
 *            constraints 5m ≤ ⌊(n-4)/2⌋ and m ≤ n-⌊(n-4)/2⌋
 * Range:     u8 n <= 19, u16 n <= 30, u32 n <= 51, u64 n <= 95, u128 n <= 181,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    /* number of elements that are ≤ (n-4)/2 */
    idx_t low = (n - 4) / 2;
    idx_t high = n - low;
    seq_t result = 0;
    idx_t mmax = low / 5;
    if (high < mmax)
        mmax = high;

    for (idx_t m = 1; m <= mmax; ++m) {
        idx_t need_low = 5 * m;
        seq_t low_comb  = binom(low,  need_low);
        seq_t high_comb = binom(high, m);
        result += low_comb * high_comb;
    }
    return result;
}
