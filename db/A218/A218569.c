/*
 * A218569  Number of partitions p of n such that max(p)-min(p) = 6.
 * https://oeis.org/A218569
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that max(p)-min(p) = 6 .
 *            The algorithm pre‑computes all values up to the maximum
 *            requested index in one sweep. For each possible minimum part m
 *            the number of partitions of the remaining sum R = n - 2*m - 6
 *            into parts between m and m+6 is counted with a classi...
 * Range:     u8 n <= 25, u16 n <= 70, u32 n <= 480, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 937 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------------
   Compute a(0 … N) (inclusive) and store the results in *out.
   Assumes *out is zeroed before the call.
   -------------------------------------------------------------------- */
static void compute_to(idx_t N, seq_t *out)
{
    /* Minimum possible remainder R = N - 2*1 - 6 = N - 8.
       Allocate a dp array large enough for the largest such R. */
    idx_t maxR = (N >= 8 ? N - 8 : 0);
    seq_t *dp = mem_new(seq_t, maxR + 1);
    if (!dp) return;  /* allocation failed; the caller must handle it */

    /* The largest minimum part that can occur is floor((N-6)/2). */
    idx_t maxM = (N > 6 ? (N - 6) / 2 : 0);

    for (idx_t m = 1; m <= maxM; ++m) {
        idx_t Rmax = N - 2 * m - 6;
        if (Rmax < 0) continue;

        /* Reset dp[0 … Rmax] */
        dp[0] = 1;
        for (idx_t i = 1; i <= Rmax; ++i) dp[i] = 0;

        /* Coin change DP for coins m … m+6 */
        for (idx_t coin = m; coin <= m + 6; ++coin) {
            if (coin > Rmax) break;
            for (idx_t s = coin; s <= Rmax; ++s) {
                dp[s] += dp[s - coin];
            }
        }

        /* Add contributions to the output array. */
        for (idx_t R = 0; R <= Rmax; ++R) {
            idx_t n = R + 2 * m + 6;
            out[n] += dp[R];
        }
    }
    mem_free(dp);
}

/* seq_t and idx_t are supplied by the build environment. */

/* Cached values of a(n).  They are allocated lazily and rebuilt only
   when a larger n is requested. */
static seq_t *cache   = NULL;   /* array of a(n) values */
static idx_t cache_len = 0;     /* current size of cache (number of entries) */

/* --------------------------------------------------------------------
   Ensure that the cache contains values up to at least n.
   Rebuild the cache if necessary.
   -------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n < 8) return;                /* values below offset are ignored */

    if (!cache || cache_len <= n) {
        /* Allocate a new cache big enough for the requested n.
           We also grow the cache in chunks to reduce reallocations. */
        idx_t new_len = n + 1;
        /* Allocate new cache array. */
        seq_t *new_cache = mem_new(seq_t, new_len);
        if (!new_cache) return;       /* allocation failure */

        /* Compute all values up to n. */
        compute_to(n, new_cache);

        /* Replace old cache if it existed. */
        if (cache) mem_free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }
}

/* --------------------------------------------------------------------
   Public function required by the test harness.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache && n < cache_len) return cache[n];
    /* Undefined for n < 8; return 0. */
    return 0;
}
