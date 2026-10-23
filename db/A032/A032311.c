/*
 * A032311  Number of ways to partition n labeled elements into sets of
 *          different sizes of at least 2.
 * https://oeis.org/A032311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] ∏_{k≥2} (1 + x^k / k!) which can be computed
 *            by the recurrence dp[0] = 1 for k = 2..n for j = n..k
 *            (descending) dp[j] += dp[j-k] * C(j, k) where C(j, k) = binomial
 *            coefficient
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 30, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  TODO: include types seq_t and idx_t supplied by the build. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* The function a implements the above recurrence.  */
seq_t a(idx_t n)
{
    /* Allocate DP array for values 0 .. n.  */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* memory allocation failed – fallback */

    /* initialise */
    for (idx_t i = (idx_t)0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[(idx_t)0] = (seq_t)1;

    /* Main double loop */
    for (idx_t k = (idx_t)2; k <= n; ++k)
    {
        /* Compute initial binomial C(n, k) */
        seq_t binom = (seq_t)1;
        for (idx_t i = (idx_t)1; i <= k; ++i)
        {
            binom = binom * (seq_t)(n - k + i) / (seq_t)i;
        }

        /* Descend j from n down to k */
        for (idx_t j = n; j >= k; )
        {
            dp[j] += dp[j - k] * binom;
            if (j == k) break;          /* avoid underflow of unsigned type */
            binom = binom * (seq_t)(j - k) / (seq_t)j;
            --j;
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
