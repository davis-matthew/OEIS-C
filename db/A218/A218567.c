/*
 * A218567  Number of partitions p of n such that max(p)-min(p) = 4.
 * https://oeis.org/A218567
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose largest part minus *
 *            smallest part equals 4. The function is implemented by dynamic *
 *            programming. For each possible smallest part m (m ≥ 1) the *
 *            partitions counted are those expressing n-2m-4 as a sum of parts
 *            from * the set {m, m+1, m+2, m+3, m+4}. All va...
 * Range:     u8 n <= 26, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* array of a[0..cache_size-1] */
static idx_t cache_size = 0;  /* number of elements in cache */

static void recompute_seq(idx_t max_n)
{
    /* Allocate a new cache (or fail and keep the old one) */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[(size_t)(max_n + 1)];
#else
    seq_t *new_cache = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
#endif
    if (!new_cache) return;               /* allocation failure */

    /* Initialise all entries to 0 */
    for (idx_t i = 0; i <= max_n; ++i) new_cache[i] = (seq_t)0;

    /* Temporary array for the DP; reused for each minimal part m */
#ifdef __cplusplus
    seq_t *dp = new seq_t[(size_t)(max_n + 1)];
#else
    seq_t *dp = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
#endif
    if (!dp) {
#ifdef __cplusplus
        delete[] new_cache;
#else
        free(new_cache);
#endif
        return;
    }
    for (idx_t i = 0; i <= max_n; ++i) dp[i] = (seq_t)0;

    /* For each possible minimal part m with 2m+4 ≤ max_n */
    idx_t max_m = (max_n >= 6) ? (max_n - 4) / 2 : 0;
    for (idx_t m = 1; m <= max_m; ++m) {
        idx_t base = 2 * m + 4;           /* n = base + S */
        idx_t maxS = max_n - base;       /* largest remaining sum */

        /* Reset dp[0..maxS] */
        for (idx_t i = 0; i <= maxS; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Unbounded knapsack over the five part sizes */
        for (idx_t coin = m; coin <= m + 4; ++coin) {
            if (coin > maxS) continue;
            for (idx_t sum = coin; sum <= maxS; ++sum)
                dp[sum] += dp[sum - coin];
        }

        /* Add contributions to the global cache */
        for (idx_t s = 0; s <= maxS; ++s)
            new_cache[base + s] += dp[s];
    }

    /* Free old cache and dp */
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    /* Replace the old cache */
    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_size = max_n + 1;
}

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 6) define the value as 0 */
    if (n < 6) return (seq_t)0;

    /* Recompute cache if necessary */
    if (n >= cache_size)
        recompute_seq(n);

    return cache[n];
}
