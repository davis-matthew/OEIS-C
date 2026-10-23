/*
 * A029432  Expansion of 1/((1-x^7)*(1-x^8)*(1-x^9)*(1-x^10)).
 * https://oeis.org/A029432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 7, 8, 9 and 10
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed values. */
static seq_t *dp = NULL;
static idx_t  max_n = 0;      /* highest n for which dp is valid  */

/* Recompute all values up to "upto" (inclusive). */
static void recompute(idx_t upto)
{
    const idx_t coins[4] = {7, 8, 9, 10};
    idx_t i, j;

    /* initialise: a[0] = 1, others 0 */
    for (i = 1; i <= upto; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* standard coin‑change DP with unlimited coins */
    for (i = 0; i < 4; ++i) {
        idx_t coin = coins[i];
        for (j = coin; j <= upto; ++j)
            dp[j] = dp[j] + dp[j - coin];
    }
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Allocate or extend storage if necessary. */
    if (dp == NULL) {
#if defined(__cplusplus)
        dp = new seq_t[n + 1];
#else
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
#endif
        max_n = n;
        recompute(max_n);
    } else if (n > max_n) {
#if defined(__cplusplus)
        seq_t *newdp = new seq_t[n + 1];
        delete[] dp;
        dp = newdp;
#else
        seq_t *newdp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        free(dp);
        dp = newdp;
#endif
        max_n = n;
        recompute(max_n);
    }

    /* If n <= max_n return already‑computed value. */
    return dp[n];
}
