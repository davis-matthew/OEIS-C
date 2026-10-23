/*
 * A048048  Number of nonempty subsets of {1,2,...,n} in which exactly 5/6 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{t=1}^{⌊n/6⌋} C(⌊(n+1)/3⌋, 5t) * C(n - ⌊(n+1)/3⌋, t),
 *            summing only over t for which 5t ≤ ⌊(n+1)/3⌋ and t ≤ n -
 *            ⌊(n+1)/3⌋
 * Range:     u8 n <= 19, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t m = (n + 1) / 3;                /* floor((n+1)/3) */
    idx_t tmax = n / 6;                   /* upper bound from 6t ≤ n */
    if (tmax > m / 5) tmax = m / 5;       /* enforce 5t ≤ m       */
    if (tmax > n - m) tmax = n - m;       /* enforce t ≤ n-m      */

    if (tmax < 1) return 0;               /* no admissible t       */

    seq_t ans = 0;
    for (idx_t t = 1; t <= tmax; ++t)
    {
        seq_t c1 = binom(m, 5 * t);
        seq_t c2 = binom(n - m, t);
        ans += c1 * c2;
    }
    return ans;
}
