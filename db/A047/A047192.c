/*
 * A047192  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1} binomial(⌊(n-2)/2⌋, 5m) * binomial(n-⌊(n-2)/2⌋,
 *            m) (sum over all m with 5m ≤ ⌊(n-2)/2⌋ and m ≤ n-⌊(n-2)/2⌋)
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 12 no subset satisfies the 5/6 condition. */
    if (n < 12)
        return 0;

    /* L = floor((n-2)/2) */
    idx_t L = (n - 2) / 2;

    idx_t low_cnt  = L;          /* number of elements ≤ L            */
    idx_t high_cnt = n - L;      /* number of elements  > L            */

    /* maximum m such that 5m ≤ low_cnt and m ≤ high_cnt */
    idx_t mmax = low_cnt / 5;
    if (mmax > high_cnt)
        mmax = high_cnt;

    seq_t sum = 0;
    for (idx_t m = 1; m <= mmax; ++m)
    {
        seq_t c_low  = binomial(low_cnt, 5 * m);
        seq_t c_high = binomial(high_cnt, m);
        sum += c_low * c_high;
    }
    return sum;
}
