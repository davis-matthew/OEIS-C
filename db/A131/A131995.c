/*
 * A131995  Number of partitions of n into powers of 2 or of 3.
 * https://oeis.org/A131995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into powers of 2 or powers of 3
 *            (1-x)/∏_{k≥0} (1-x^{2^k})(1-x^{3^k})
 * Range:     u8 n <= 22, u16 n <= 79, u32 n <= 417, u64 n <= 3660,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Global cache for the dynamic programming array */
static seq_t *dp = NULL;          /* array of partition counts */
static size_t dp_cap = 0;         /* allocated size of dp */

static void ensure_dp_cap(idx_t n)
{
    /* Ensure dp has space for indices 0 .. n (inclusive) */
    if ((idx_t)(dp_cap) <= n)
    {
        dp_cap = (size_t)((n + 1) * 2);   /* a little extra to reduce reallocs */
        seq_t *tmp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        if (dp) free(dp);
        dp = tmp;
    }
}

/* the required OEIS function */
seq_t a(idx_t n)
{
    /* Powers of 2 and 3 up to n */
    idx_t coins[64];
    size_t coin_cnt = 0;

    /* 1 is allowed once */
    coins[coin_cnt++] = 1;

    /* powers of 2, start from 2^1 = 2 */
    for (idx_t p = 2; p <= n; p <<= 1)
        if (p > 0)  /* prevent overflow on very large n (not needed here) */
            coins[coin_cnt++] = p;

    /* powers of 3, start from 3^1 = 3 */
    for (idx_t p = 3; p <= n; p *= 3)
        if (p > 0)
            coins[coin_cnt++] = p;

    /* DP: number of partitions using the above coins */
    ensure_dp_cap(n);

    /* initialize dp[0..n] to 0, set dp[0] = 1 */
    memset(dp, 0, (n + 1) * sizeof(seq_t));
    dp[0] = 1;

    /* usual coin‑change DP */
    for (size_t c = 0; c < coin_cnt; ++c)
    {
        idx_t coin = coins[c];
        for (idx_t j = coin; j <= n; ++j)
        {
            dp[j] = dp[j] + dp[j - coin];
        }
    }

    return dp[n];
}
