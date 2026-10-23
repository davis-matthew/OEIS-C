/*
 * A180035  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1+x)/(1-5*x-3*x^2).
 * https://oeis.org/A180035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) with a(0) = 1, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"

/* Cache for already computed values. */
static idx_t cache_cap   = 0;          /* allocated length of cache */
static seq_t *cache      = NULL;      /* array of computed terms   */
static idx_t cache_max   = (idx_t)-1;  /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold the required index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base terms on the first call. */
    if (cache_max == (idx_t)-1) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1)
            cache[1] = (seq_t)6;
        cache_max = (cache_cap > 1) ? 1 : 0;
    }

    /* If the requested term is already computed, return it. */
    if (n <= cache_max)
        return cache[n];

    /* Compute terms up to n, using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)3 * cache[i - 2];
    }
    cache_max = n;
    return cache[n];
}
