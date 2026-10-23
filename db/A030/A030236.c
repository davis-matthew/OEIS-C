/*
 * A030236  Cycle-path coverings of a family of digraphs.
 * https://oeis.org/A030236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1,2,7,18 for n = 0,1,2,3; a(n) = 3*a(n-1) - a(n-2) +
 *            a(n-3) for n >= 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (any order of calls) */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;   /* number of terms currently stored */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* initialise cache on the first call */
    if (cache_cap == 0) {
        cache_cap = 4;                       /* space for first four terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure -> 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)18;
        cache_size = 4;
    }

    /* enlarge allocation if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap < required)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure -> 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    if (required > cache_size) {
        for (size_t i = cache_size; i < required; ++i) {
            /* recurrence holds for i >= 4 (i is size_t so >=4 here) */
            cache[i] = 3 * cache[i-1] - cache[i-2] + cache[i-3];
        }
        cache_size = required;
    }

    return cache[(size_t)n];
}
