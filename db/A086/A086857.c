/*
 * A086857  a(0) = 0, a(1) = 1; for n > 1, a(n) = (a(n-1) + a(n-2) + 1)^2.
 * https://oeis.org/A086857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)+a(n-2)+1)^2, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoises computed values; expands storage as needed. */

static seq_t *cache = NULL;       /* cached sequence values */
static size_t cache_size = 0;     /* number of cached entries [0, cache_size-1] */

seq_t a(idx_t n)
{
    /* convert index to a size_t for array access */
    size_t idx = (size_t)n;

    /* grow cache if the requested index is not yet cached */
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – return a benign value */
            return (seq_t)0;
        }
        cache = new_cache;

        /* initialise base cases if they have not been set yet */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            cache_size = 1;
            if (idx >= 1) {
                cache[1] = (seq_t)1;
                cache_size = 2;
            }
        } else if (cache_size == 1) {
            cache[1] = (seq_t)1;
            cache_size = 2;
        }

        /* compute all missing terms up to idx */
        for (size_t k = cache_size; k <= idx; ++k) {
            seq_t sum = cache[k - 1] + cache[k - 2] + (seq_t)1;
            cache[k] = sum * sum;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
