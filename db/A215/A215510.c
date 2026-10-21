/*
 * A215510  a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3) with a(0)=0, a(1)=7,
 *          a(2)=35.
 * https://oeis.org/A215510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3); a(0)=0, a(1)=7, a(2)=35
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) from the linear recurrence a(n) = 7*a(n-1) - 14*a(n-2) + 7*a(n-3)   */
/* Handles out‑of‑order calls by caching previously computed terms. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed values */
    static seq_t *cache      = NULL;   /* array of computed terms */
    static size_t cache_size = 0;      /* allocated size of the array */
    static idx_t  last_computed = 2;   /* largest n that has been computed */

    /* First call: allocate space for the first four indices (0..3) and
       initialise the known seed values. */
    if (cache == NULL) {
        size_t init_size = 4;                     /* includes indices 0..3 */
        cache = (seq_t *)calloc(init_size, sizeof(seq_t));
        if (!cache) abort();                     /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)35;
        cache[3] = (seq_t)147;                   /* a(3) from recurrence */
        cache_size = init_size;
    }

    /* Return immediately for the first three terms. */
    if (n <= 2)
        return cache[n];

    /* Grow the cache if the requested index is beyond the current capacity. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_cap = ((size_t)n + 1) * 2;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1; /* overflow guard */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        /* Zero any new entries to avoid garbage. */
        for (size_t i = cache_size; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }

    /* Compute all missing terms up to the requested index. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)7  * cache[i - 1]
                 - (seq_t)14 * cache[i - 2]
                 + (seq_t)7  * cache[i - 3];
    }
    last_computed = n;

    return cache[n];
}
