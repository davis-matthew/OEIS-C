/*
 * A099200  Number of partitions of n into distinct odd triangular numbers.
 * https://oeis.org/A099200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   compute a(n) as the number of partitions of n into distinct odd
 *            triangular numbers, using dynamic programming
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array: dp[s] = number of subsets summing to s */
#if defined(__cplusplus)
    seq_t *dp = new seq_t[n + 1];
#else
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
#endif
    if (!dp) return 0;  /* safety: return 0 on allocation failure */

    /* initialise */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* empty subset sums to 0 */

    /* enumerate odd triangular numbers ≤ n:
       T(k) = k*(k+1)/2 is odd iff k ≡ 1 or 2 (mod 4)   */
    for (idx_t k = 1; ; ++k)
    {
        seq_t t_val = (seq_t)k * ((seq_t)k + 1) / 2;
        if (t_val > (seq_t)n) break;          /* beyond target */

        if ((k & 3ULL) == 1 || (k & 3ULL) == 2)
        {
            idx_t t = (idx_t)t_val;           /* t ≤ n ≤ 10000 */

            /* update DP from back to avoid reuse of a number */
            for (idx_t s = n; s >= t; --s)
            {
                dp[s] += dp[s - t];
                if (s == t) break;            /* avoid wrap‑around of unsigned type */
            }
        }
    }

    seq_t result = dp[n];

#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
