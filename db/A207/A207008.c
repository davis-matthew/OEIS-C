/*
 * A207008  Number of n X 1 0..2 arrays avoiding the patterns z z+1 z or z z-1
 *          z in any row or column.
 * https://oeis.org/A207008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) + 2*a(n-4); Initial
 *            values: a(1)=3, a(2)=9, a(3)=23, a(4)=61
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache of computed terms: cache[0] holds a(1), cache[1] holds a(2), etc. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms currently stored */
    static size_t cache_cap  = 0;    /* allocated capacity of cache */

    /* first call – initialise the cache with the first four terms */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();              /* allocation failed */
        cache[0] = 3;     /* a(1) */
        cache[1] = 9;     /* a(2) */
        cache[2] = 23;    /* a(3) */
        cache[3] = 61;    /* a(4) */
        cache_len = 4;
    }

    /* extend the cache if the requested n is larger */
    if ((idx_t)cache_len < n) {
        size_t needed = (size_t)n;

        /* grow the allocation if necessary */
        if (needed > cache_cap) {
            cache_cap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
        }

        /* compute missing terms using the recurrence */
        for (size_t k = cache_len; k < needed; ++k) {
            seq_t val;
            if (k == 0) val = 3;
            else if (k == 1) val = 9;
            else if (k == 2) val = 23;
            else if (k == 3) val = 61;
            else
                val = 3*cache[k-1] - 2*cache[k-2] + 2*cache[k-3] + 2*cache[k-4];
            cache[k] = val;
        }
        cache_len = needed;
    }

    return cache[n-1];
}
