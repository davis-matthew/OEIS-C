/*
 * A218440  Hankel transform of A078818.
 * https://oeis.org/A218440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 14*a(n-1) - 6*a(n-2) + 14*a(n-3) - a(n-4) for n≥4 with
 *            a(0)=10, a(1)=135, a(2)=1844, a(3)=25145
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                  // for size_t
#include "../utils/ensure_cache.h"   // ensures cache growth

static seq_t *cache = NULL;          // cache for computed terms
static size_t cache_cap = 0;          // allocated cache capacity (elements)
static idx_t  last_computed = 0;      // highest index that has been computed

seq_t a(idx_t n) {
    /* Allocate (and initialise) the first four terms if this is the first call. */
    if (cache == NULL) {
        /* Ensure room for indices 0..3. */
        ensure_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)135;
        cache[2] = (seq_t)1844;
        cache[3] = (seq_t)25145;
        last_computed = 3;
    } else if ((size_t)n >= cache_cap) {
        /* Grow the cache to accommodate index n. */
        ensure_cache(n, &cache, &cache_cap);
    }

    /* If the value is already known, return it immediately. */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute new terms up to index n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For i ≥ 4 the recurrence applies. */
        cache[i] = 14 * cache[i - 1]
                 - 6  * cache[i - 2]
                 + 14 * cache[i - 3]
                 -  cache[i - 4];
    }

    last_computed = n;
    return cache[n];
}
