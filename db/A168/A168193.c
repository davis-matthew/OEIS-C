/*
 * A168193  a(n) = a(n-1) + a(n-2) + 4, with a(0)=0, a(1)=2.
 * https://oeis.org/A168193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 4, with a(0)=0 and a(1)=2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;    /* number of entries currently stored   */
static idx_t cache_cap   = 0;    /* allocated capacity of cache          */

/* Expand the cache, at least up to 'needed' entries */
static void grow_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;
    /* Double capacity until sufficient, capped to needed plus some room */
    if (cache_cap == 0)
        cache_cap = 2;
    while (cache_cap <= needed)
        cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    /* Safety: if realloc fails, the program will terminate; not required
       to handle explicit errors for this context. */
}

/* Compute sequence value a(n) with memoisation, arbitrary order of calls */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return 0;
    if (n == 1)
        return 2;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        cache_size = 2;
    }

    /* Extend cache up to n using recurrence */
    if (cache_size <= n) {
        grow_cache(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = a(i-1) + a(i-2) + 4 */
            cache[i] = cache[i - 1] + cache[i - 2] + 4;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
