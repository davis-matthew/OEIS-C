/*
 * A014980  a(n+1) = floor(a(n)/2) * ceiling(a(n)/2), a(0) = 5.
 * https://oeis.org/A014980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 5;
 *          a(n+1) = floor(a(n)/2) * ceil(a(n)/2)   for n ≥ 0
 *
 * Implements the recurrence efficiently with memoisation.
 * The results are stored in a static cache so that calls with
 * arbitrary order of n are handled in O(max(n)) time overall.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Cache for previously computed terms.  The cache contains
     * a(0) ... a(cache_len-1).  cache_len is the number of elements
     * currently stored.  cache_cap is the allocated capacity. */
    static seq_t *cache     = NULL;
    static idx_t cache_len  = 0;
    static size_t cache_cap = 0;

    /* Allocate and initialise the cache for the first call. */
    if (cache == NULL) {
        /* Start with a reasonable initial capacity. */
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            /* Allocation failed – abort. */
            abort();
        }
        cache[0] = (seq_t)5;
        cache_len = 1;
    }

    /* Grow the cache if needed. */
    size_t required = (size_t)(n + 1);
    if (required > cache_cap) {
        cache_cap = required * 2;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            abort();
        }
    }

    /* Compute missing terms up to index n. */
    for (; cache_len <= n; ++cache_len) {
        seq_t prev = cache[cache_len - 1];
        seq_t low  = prev >> 1;          /* floor(prev/2)   */
        seq_t high = (prev + 1) >> 1;     /* ceil(prev/2)    */
        cache[cache_len] = low * high;
    }

    return cache[n];
}
