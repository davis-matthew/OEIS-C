/*
 * A029324  Expansion of 1/((1-x^3)*(1-x^9)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A029324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 3,9,11,12 * The
 *            implementation uses static 2‑D DP (flattened) with * dp[n][i] =
 *            partitions of n using the first (i+1) part sizes. * Recurrence:
 *            dp[n][i] = dp[n][i-1] + (n>=coin[i]?dp[n-coin[i]][i]:0) * i =
 *            0..3 for coins 3,9,11,12. dp[n][3] is the desir...
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* flattened 1‑D array of size cap*4 */
static idx_t max_n = 0;           /* highest n finished */
static size_t dp_cap = 0;          /* allocated number of rows */

/* Coin denominations in order used for DP */
static const idx_t coins[4] = {3, 9, 11, 12};

seq_t a(idx_t n)
{
    /* allocate / grow storage so that it contains rows 0..n */
    if (!dp) {
        /* first allocation */
        dp_cap = (size_t)n + 1;
        dp = (seq_t *)malloc(dp_cap * 4 * sizeof(seq_t));
        if (!dp) abort();

        /* initialise row 0: one way to partition zero with any coin set */
        for (int i = 0; i < 4; ++i) dp[i] = 1;
        max_n = 0;
    } else if ((size_t)n >= dp_cap) {
        /* enlarge array */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < dp_cap * 2) new_cap = dp_cap * 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * 4 * sizeof(seq_t));
        if (!tmp) abort();
        /* copy existing rows */
        for (size_t i = 0; i <= max_n; ++i)
            for (int j = 0; j < 4; ++j)
                tmp[i * 4 + j] = dp[i * 4 + j];
        free(dp);
        dp = tmp;
        dp_cap = new_cap;
    }

    /* compute missing rows up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        size_t idx_cur = (size_t)cur * 4;
        /* i = 0 (only coin 3) */
        if (cur >= coins[0])
            dp[idx_cur + 0] = dp[idx_cur + 0] + dp[(cur - coins[0]) * 4 + 0];
        else
            dp[idx_cur + 0] = 0;

        /* i = 1 (coins 3,9) */
        if (cur >= coins[1])
            dp[idx_cur + 1] = dp[idx_cur + 0] + dp[(cur - coins[1]) * 4 + 1];
        else
            dp[idx_cur + 1] = dp[idx_cur + 0];

        /* i = 2 (coins 3,9,11) */
        if (cur >= coins[2])
            dp[idx_cur + 2] = dp[idx_cur + 1] + dp[(cur - coins[2]) * 4 + 2];
        else
            dp[idx_cur + 2] = dp[idx_cur + 1];

        /* i = 3 (coins 3,9,11,12) */
        if (cur >= coins[3])
            dp[idx_cur + 3] = dp[idx_cur + 2] + dp[(cur - coins[3]) * 4 + 3];
        else
            dp[idx_cur + 3] = dp[idx_cur + 2];
    }

    max_n = n;
    return dp[(size_t)n * 4 + 3];
}
