/*
 * A152105  a(n) = (10^n + 6^n)/2.
 * https://oeis.org/A152105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 6^n)/2 Recurrence: a(n) = 16*a(n-1) - 60*a(n-2),
 *            with a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_cache, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_cache.h"
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute a(n) = (10^n + 6^n)/2 for any n ≥ 0.
   Uses the linear recurrence and memoises computed values
   in a static cache so subsequent calls are O(1). */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base values the first time. */
    cache[0] = 1;
    if (n >= 1) {
        cache[1] = 8;
    }

    /* Compute any needed values using the recurrence. */
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            /* a(i) = 16*a(i-1) - 60*a(i-2) */
            cache[i] = (seq_t)16 * cache[i-1] - (seq_t)60 * cache[i-2];
        }
    }

    return cache[n];
}
