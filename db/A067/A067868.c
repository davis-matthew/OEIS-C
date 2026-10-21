/*
 * A067868  a(n) = a(n-1) + a(floor(n/2))^2 for n > 0, a(0) = 1.
 * https://oeis.org/A067868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/2))^2 (a(0) = 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;   /* number of values stored (up to cache_len-1) */
    static size_t  cache_cap = 0;   /* allocated capacity, in cache_len units */

    /* initialise cache lazily */
    if (cache == NULL) {
        cache_cap = 16;
        cache_len = 0;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
    }

    /* grow the cache if necessary */
    if ((idx_t)(n + 1) > cache_len) {
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < (size_t)(n + 1)) {
                new_cap <<= 1;           /* double capacity until it fits */
                if (new_cap == 0) new_cap = 1;  /* paranoia for overflow */
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute all missing terms up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;                /* a(0) = 1 */
            } else {
                seq_t prev = cache[i - 1];
                seq_t h    = cache[i / 2];
                cache[i] = prev + h * h;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
