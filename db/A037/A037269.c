/*
 * A037269  Expansion of (1+x^30)/((1-x^4)(1-x^5)(1-x^6)(1-x^12)).
 * https://oeis.org/A037269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n using parts {4,5,6,12} plus
 *            number of partitions of n-30 using parts {4,5,6,12}; i.e. the
 *            coefficient of x^n in (1+x^30)/((1-x^4)(1-x^5)(1-x^6)(1-x^12))
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached DP table.  */
static seq_t *dp = NULL;
static idx_t   dp_size = 0;

/* Re‑compute all dp[0..n] from scratch.  */
static void recompute_dp(idx_t n)
{
    /* Allocate or extend the table.  */
    if (dp == NULL || dp_size <= n) {
        seq_t *p = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        dp = p;
        dp_size = n + 1;
    }

    /* Reset all entries to zero and set the empty partition.  */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;  /* 1 way to make sum 0: use no parts */

    /* Unordered partitions with parts 4,5,6,12.  */
    const idx_t parts[4] = {4, 5, 6, 12};
    for (int k = 0; k < 4; ++k) {
        idx_t p = parts[k];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }
}

/* Public function definition.  */
seq_t a(idx_t n)
{
    recompute_dp(n);
    seq_t res = dp[n];
    if (n >= 30) {
        res += dp[n - 30];
    }
    return res;
}
