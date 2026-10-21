/*
 * A212336  Expansion of 1/(1 - 23*x + 23*x^2 - x^3).
 * https://oeis.org/A212336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   free_cache, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 23*a(n-1) - 23*a(n-2) + a(n-3),
   with initial values a(0)=1, a(1)=23, a(2)=506 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_cache.h"

/* Compute just‑in‑time with memoisation.
   The cache is grown only when a larger index is requested. */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache   = NULL;      /* array of computed values   */
    static idx_t  last_idx = (idx_t)-1;/* highest index computed */

    /* Initialise cache if this is the first call */
    if (last_idx == (idx_t)-1) {
        last_idx = 2;                                   /* we know a(0..2) */
        cache   = seq_alloc_new_array((size_t)3);       /* size = last_idx+1 */
        cache[0] = 1;
        cache[1] = 23;
        cache[2] = 506;
    }

    /* If the requested index is already cached, return it */
    if (n <= last_idx) return cache[(size_t)n];

    /* ------------------------------------------------------------
       Grow the cache to size (n+1).  Allocate a new larger array,
       copy the old data, compute the missing terms, then discard
       the old array.  This guarantees that the cache size never
       decreases, so no intermediate values are lost.
       ------------------------------------------------------------ */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = seq_alloc_new_array(new_size);

    /* Copy existing values */
    for (size_t i = 0; i <= (size_t)last_idx; ++i)
        new_cache[i] = cache[i];

    /* Compute any new terms using the recurrence */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        seq_t term1 = 23 * new_cache[(size_t)(i - 1)];
        seq_t term2 = 23 * new_cache[(size_t)(i - 2)];
        new_cache[(size_t)i] = term1 - term2 + new_cache[(size_t)(i - 3)];
    }

    /* Free the old cache and replace it */
    free_cache(cache);
    cache   = new_cache;
    last_idx = n;

    return cache[(size_t)n];
}
