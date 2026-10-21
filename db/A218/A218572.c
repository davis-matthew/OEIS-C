/*
 * A218572  Number of partitions p of n such that max(p)-min(p) = 9.
 * https://oeis.org/A218572
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n whose largest part minus *
 *            smallest part equals 9. This is implemented by summing over all
 *            * possible minimal parts k (k ≥ 1) and adding the number of
 *            partitions * of the remaining sum using the ten consecutive part
 *            sizes k,…,k+9. * The result is accumulated into...
 * Range:     u8 n <= 26, u16 n <= 60, u32 n <= 241, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for computed values. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;          /* number of entries stored in cache */

/* Dynamically sized DP array reused for each k. */
static seq_t *dp = NULL;
static idx_t dp_size = 0;

static void compute_upto(idx_t N)
{
    if (N < 11) return;              /* nothing needed below the offset */

    /* Resize DP array if necessary. */
    if (dp_size < N + 1) {
        free(dp);
        dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        dp_size = N + 1;
    }

    /* Allocate new cache and copy old values. */
    seq_t *new_cache = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (cache) {
        idx_t copy_len = cache_len < (N + 1) ? cache_len : N + 1;
        for (idx_t i = 0; i < copy_len; ++i) new_cache[i] = cache[i];
        for (idx_t i = cache_len; i <= N; ++i) new_cache[i] = 0;
    } else {
        for (idx_t i = 0; i <= N; ++i) new_cache[i] = 0;
    }

    /* Main loop over minimal part k. */
    idx_t Kmax = (N - 9) / 2;     /* largest k with 2k+9 ≤ N */
    for (idx_t k = 1; k <= Kmax; ++k) {
        idx_t res_max = N - 2 * k - 9;      /* maximum residual sum to be formed */

        /* Reset DP array for this k. */
        for (idx_t i = 0; i <= res_max; ++i) dp[i] = 0;
        dp[0] = 1;

        /* Coin‑change DP for the ten consecutive coins k … k+9. */
        for (int j = 0; j < 10; ++j) {
            idx_t coin = k + j;
            for (idx_t i = coin; i <= res_max; ++i) {
                dp[i] += dp[i - coin];
            }
        }

        /* Add contributions to the cache. */
        idx_t base = 2 * k + 9;
        for (idx_t t = 0; t <= res_max; ++t) {
            new_cache[base + t] += dp[t];
        }
    }

    /* Replace old cache. */
    free(cache);
    cache = new_cache;
    cache_len = N + 1;
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 11) return 0;                    /* no terms below the offset */

    if (!cache || n >= cache_len)            /* enlarge cache if needed */
        compute_upto(n);

    return cache[n];
}
