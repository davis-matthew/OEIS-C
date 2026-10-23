/*
 * A022326  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 12.
 * https://oeis.org/A022326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = a(n-1) + a(n-2) + 1,  a(0)=1, a(1)=12 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache       = NULL;
    static size_t cache_size  = 0;   /* number of elements stored      */
    static size_t cache_cap   = 0;   /* allocated capacity             */

    /* Convert idx_t to a type suitable for array indexing */
    size_t target = (size_t)n;

    /* Return cached value if it already exists */
    if (target < cache_size)
        return cache[target];

    /* Resize the cache if necessary */
    if (target + 1 > cache_cap) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to the required index */
    for (size_t i = cache_size; i <= target; ++i) {
        if (i == 0) {
            cache[i] = 1;
        } else if (i == 1) {
            cache[i] = 12;
        } else {
            cache[i] = cache[i-1] + cache[i-2] + 1;
        }
    }

    cache_size = target + 1;
    return cache[target];
}
