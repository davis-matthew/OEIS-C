/*
 * A030038  a(n+1) = Sum_{k=0..floor(3*n/5)} a(k) * a(n-k).
 * https://oeis.org/A030038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 54, u128 n <= 102,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n+1) = Σ_{k=0..⌊3n/5⌋} a(k) * a(n-k) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise first term */
    if (cache_len == 0) {
        /* allocate space for at least one element */
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    /* compute missing terms */
    for (idx_t m = cache_len; m <= n; ++m) {
        idx_t limit = (idx_t)((3ull * (m - 1ull)) / 5ull);  /* floor(3*(m-1)/5) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= limit; ++k) {
            sum += cache[k] * cache[m - 1 - k];
        }
        cache[m] = sum;
    }
    cache_len = n + 1;  /* we now have terms up to n */

    return cache[n];
}
