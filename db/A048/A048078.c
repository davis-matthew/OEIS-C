/*
 * A048078  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n+2)/3.
 * https://oeis.org/A048078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1} C(⌊(n+2)/3⌋, 3m) * C(n − ⌊(n+2)/3⌋, 2m) where the
 *            sum stops when 3m > ⌊(n+2)/3⌋ or 2m > n − ⌊(n+2)/3⌋
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Number of elements ≤ (n+2)/3. */
    idx_t low  = (n + 2) / 3;          /* floor((n+2)/3) */
    idx_t high = n - low;              /* elements > (n+2)/3 */

    /* Max m allowed by each part. */
    idx_t mmax_low  = low  / 3;        /* 3m ≤ low  → m ≤ low/3  */
    idx_t mmax_high = high / 2;        /* 2m ≤ high → m ≤ high/2 */

    /* Overall upper bound for m. */
    idx_t mmax = (mmax_low < mmax_high) ? mmax_low : mmax_high;

    seq_t ans = 0;
    for (idx_t m = 1; m <= mmax; ++m)
    {
        seq_t c1 = binom(low, 3 * m);   /* C(low, 3m) */
        seq_t c2 = binom(high, 2 * m);  /* C(high, 2m) */
        ans += c1 * c2;
    }
    return ans;
}
