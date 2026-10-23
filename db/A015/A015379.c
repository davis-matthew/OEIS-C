/*
 * A015379  Gaussian binomial coefficient [ n,9 ] for q=-7.
 * https://oeis.org/A015379
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,9] for q = -7 (OEIS
 *            A015379)
 * Range:     i8 n <= 9, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>

seq_t a(idx_t n)
{
    /* The coefficient is only defined for n >= 9.
       For indices below the offset the sequence is undefined – return 0. */
    if (n < 9)
        return (seq_t)0;

    /* k = 9, fixed.  The recurrence
         [n,k] = [n-1,k] + q^{n-k} * [n-1,k-1]
       with q = -7 generates the value for any n progressively. */
    const idx_t  k   = 9;
    const seq_t q    = (seq_t)-7;
    seq_t        dp[10]; /* dp[j] = [current_n , j]  */
    for (idx_t j = 0; j <= k; ++j)
        dp[j] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* pow_qk holds q^{current_n - k}.  It is only needed once current_n >= k. */
    seq_t pow_qk = (seq_t)1;   /* at n = k  -> power = q^{0} = 1 */

    for (idx_t current = 1; current <= n; ++current)
    {
        /* Limit j to the smaller of k and current (values > current are 0). */
        idx_t jmax = (current < k) ? current : k;

        /* Start pow with q^{current-jmax}.
           When current < k, jmax == current so exponent is 0 -> pow = 1.
           When current >= k, jmax == k so exponent = current-k.  */
        seq_t pow = (current < k) ? (seq_t)1 : pow_qk;

        /* Update each dp[j] from jmax down to 1.
           The factor q^{current-j} grows by a factor of q when j decreases. */
        for (idx_t j = jmax; j >= 1; --j)
        {
            dp[j] = dp[j] + pow * dp[j - 1];
            if (j > 1)
                pow = pow * q;   /* prepare pow for next smaller j */
        }

        /* Advance pow_qk for the next iteration of outer loop. */
        if (current >= k)
            pow_qk = pow_qk * q;
    }

    return dp[9];
}
