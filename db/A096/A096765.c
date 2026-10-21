/*
 * A096765  Number of partitions of n into distinct parts, the least being 1.
 * https://oeis.org/A096765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts with least
 *            part 1 (equivalently, number of partitions of n-1 into distinct
 *            parts all ≥ 2)
 * Range:     u8 n <= 34, u16 n <= 86, u32 n <= 250, u64 n <= 814,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 */
    if (n == 0) return (seq_t)0;

    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* largest n for which cache is valid */

    /* if the requested n is already cached, just return it */
    if (n <= cached_max)
        return cache[n];

    /* Need to recompute cache up to n */
    /* Allocate temporary array for subset–sum counts up to n-1 */
    idx_t target = n - 1;                    /* largest sum needed */
    size_t dp_size = (size_t)target + 1;     /* indices 0..target */
    seq_t *dp = (seq_t *)calloc(dp_size, sizeof(seq_t));
    if (!dp) return (seq_t)0;                /* silently handle allocation failure */

    dp[0] = (seq_t)1;                        /* one way to sum to 0 (empty set) */

    /* Dynamic programming: for each part j >= 2, update dp[s] for s>=j */
    for (idx_t j = 2; j <= target; ++j) {
        for (idx_t s = target; s >= j; --s) {
            dp[s] += dp[s - j];
        }
    }

    /* Build cache array of length n+1 (indices 0..n) */
    size_t cache_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    if (!new_cache) {
        free(dp);
        return (seq_t)0;   /* allocation failure */
    }
    new_cache[0] = (seq_t)0;          /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i)
        new_cache[i] = dp[i - 1];     /* a(i) = dp[i-1] */

    free(dp);                          /* dp no longer needed */

    /* Replace old cache, freeing it */
    free(cache);
    cache = new_cache;
    cached_max = n;

    return cache[n];
}
