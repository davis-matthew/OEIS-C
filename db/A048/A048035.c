/*
 * A048035  Number of nonempty subsets of {1,2,...,n} in which exactly 4/5 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{min(n/5, ⌊t/4⌋, n-t)} C(t,4m)·C(n-t,m), where t
 *            = ⌊(n-3)/3⌋ (t=0 for n≤2)
 * Range:     u8 n <= 23, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t t = 0;

    if (n > 2)
        t = n / 3 - 1;          /* floor((n-3)/3) = n/3 - 1 for n≥3 */

    idx_t k = n - t;             /* remaining elements */

    /* maximum m satisfying 5m ≤ n, 4m ≤ t, and m ≤ k */
    idx_t mmax = n / 5;
    if (t / 4 < mmax)
        mmax = t / 4;
    if (k < mmax)
        mmax = k;

    for (idx_t m = 1; m <= mmax; ++m) {
        idx_t t4m = 4 * m;
        seq_t c1 = binom(t, t4m);   /* C(t,4m)   */
        seq_t c2 = binom(k, m);     /* C(k,m)    */
        res += c1 * c2;             /* addition works for builtin and arbitrary‑precision seq_t */
    }

    return res;
}
