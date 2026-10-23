/*
 * A147958  a(n) = ((7 + sqrt(2))^n + (7 - sqrt(2))^n)/2.
 * https://oeis.org/A147958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 47*a(n-2); a(0)=1, a(1)=7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* Utility to grow a cache array and keep its capacity up to index n. */
#include "../utils/ensure_cache.h"

/* Compute the n-th term of OEIS A147958.  The function is memoised
   so that repeated calls are fast, and it works for any order of
   calls.  All arithmetic is performed directly on the seq_t type. */
seq_t a(idx_t n)
{
    /* static cache:  cache[i] holds a(i) when i < cache_used. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size of cache */
    static size_t cache_used = 0;  /* highest computed index + 1 */

    /* Initialise base values a(0)=1 and a(1)=7 if necessary. */
    if (cache_used == 0) {
        ensure_cache(0, &cache, &cache_cap);   /* need at least slot 0 */
        cache[0] = 1;
        cache_used = 1;
    }
    if (cache_used == 1) {
        ensure_cache(1, &cache, &cache_cap);   /* need slots 0..1 */
        cache[1] = 7;
        cache_used = 2;
    }

    /* If we haven't computed up to n, extend the cache and fill in */
    if (n >= cache_used) {
        ensure_cache(n, &cache, &cache_cap);   /* make sure we have slots up to n */

        for (idx_t i = cache_used; i <= n; ++i) {
            /*  a(i) = 14*a(i-1) - 47*a(i-2)  */
            cache[i] = 14 * cache[i-1] - 47 * cache[i-2];
        }
        cache_used = n + 1;   /* all indices < n+1 are now valid */
    }

    return cache[n];
}
