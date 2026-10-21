/*
 * A098743  Number of partitions of n into aliquant parts (i.e., parts that do
 *          not divide n).
 * https://oeis.org/A098743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 58, u32 n <= 156, u64 n <= 466,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of partitions of n into parts that do not divide n
   using standard dynamic programming.  For each n the allowed parts are
   all k in [1,n] with n % k != 0.  The classic coin‑change DP
   computes the count in O(n²) time.  The function memoizes each value
   so repeated calls with the same n are O(1). */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;           /* cache of computed values */
static bool *computed = NULL;         /* flags for whether cache[i] is valid */
static size_t cache_cap = 0;           /* capacity of cache arrays */

/* Ensure the cache arrays have capacity for indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        computed = (bool *)malloc(new_cap * sizeof(bool));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        computed = (bool *)realloc(computed, new_cap * sizeof(bool));
    }
    for (size_t i = cache_cap; i < new_cap; ++i)
        computed[i] = false;   /* uncomputed */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_cache_capacity(n);
    if (computed[n])
        return cache[n];

    /* Allocate DP array of size n+1 */
    seq_t *dp = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
    if (dp == NULL)   /* allocation failure: return 0 */
        return (seq_t)0;

    dp[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j)
        dp[j] = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        if (n % k == 0)      /* parts dividing n are forbidden */
            continue;
        for (idx_t j = k; j <= n; ++j) {
            dp[j] += dp[j - k];
        }
    }

    seq_t result = dp[n];
    free(dp);

    cache[n] = result;
    computed[n] = true;
    return result;
}
