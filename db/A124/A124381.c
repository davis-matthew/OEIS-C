/*
 * A124381  O.g.f.: A(x) = Sum_{n>=0} x^n*Product_{k=0..n} (1 + k*(k+1)/2*x).
 * https://oeis.org/A124381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence from OEIS A124381:
   a(n) = coefficient of x^n in 
          Σ_{k≥0} x^k ∏_{h=0..k} (1 + h(h+1)/2 * x)  */

/* Implementation uses dynamic programming.
   For each m = 1..n we maintain the coefficients of
   ∏_{h=1..m} (1 + h(h+1)/2 * x) in an array dp.
   After the update dp[k] holds the sum of products
   of all subsets of {1,…,m} of size k.
   The term x^m * ∏_{h=1..m} (...) contributes dp[r]
   to a(m+r).  When m+r == n we add to the answer. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* dp array: dp[r] is coefficient of x^r in ∏_{h=1..m} ... */
    idx_t sz = n + 1;
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp) return 0;           /* allocation failed */

    dp[0] = 1;
    for (idx_t i = 1; i < sz; ++i) dp[i] = 0;

    seq_t ans = 0;
    idx_t half = (n + 1) / 2;     /* ceil(n/2) */

    for (idx_t m = 1; m <= n; ++m)
    {
        seq_t T = ((seq_t)m * (seq_t)(m + 1)) / 2;   /* triangular number m(m+1)/2 */

        /* Update dp from high to low to avoid overwrite */
        for (idx_t r = m; r > 0; --r)
        {
            dp[r] = dp[r] + dp[r - 1] * T;
        }

        if (m >= half)
        {
            ans = ans + dp[n - m];
        }
    }

    free(dp);
    return ans;
}
