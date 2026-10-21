/*
 * A206817  Sum_{0<j<k<=n} (k!-j!).
 * https://oeis.org/A206817
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{0<j<k<=n} (k! - j!). Recurrence used: a(n) = a(n-1)
 *            + (n-1)*n! - Sum_{j=1}^{n-1} j! for n >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* offset starts at 2, below offset value is undefined */

    seq_t ans    = 0;   /* accumulated sum a(n) */
    seq_t fac    = 1;   /* factorial, starts with 0! = 1 but will become 1! first */
    seq_t sumFac = 0;   /* Σ_{j=1}^{k-1} j!  before computing term for current k */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update factorial to k! */
        fac *= (seq_t)k;

        if (k >= 2) {
            /* term = (k-1)*k! - Σ_{j=1}^{k-1} j! */
            seq_t term = (seq_t)(k - 1) * fac - sumFac;
            ans += term;
        }

        /* Add current factorial to the prefix sum for next iteration */
        sumFac += fac;
    }

    return ans;
}
