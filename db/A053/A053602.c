/*
 * A053602  a(n) = a(n-1) - (-1)^n*a(n-2), a(0)=0, a(1)=1.
 * https://oeis.org/A053602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - (-1)^n*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 24, u16 n <= 46, u32 n <= 92, u64 n <= 184,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* extend cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache;

#ifdef __cplusplus
        /* C++: use operator new to construct seq_t objects */
        new_cache = new seq_t[new_size];
#else
        /* C: use realloc (seq_t is trivially copyable) */
        new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#endif
        if (!new_cache) {
            /* allocation failed */
            exit(1);
        }

        /* copy existing values if we are resizing */
        if (cache_size > 0) {
#ifdef __cplusplus
            for (idx_t i = 0; i < cache_size; ++i) {
                new_cache[i] = cache[i];
            }
#endif
        }

        /* initialise the first two terms if they were not yet set */
        if (cache_size == 0) {
            new_cache[0] = (seq_t)0;
            if (new_size > 1) {
                new_cache[1] = (seq_t)1;
            }
            cache_size = (new_size > 1) ? 2 : 1;
        } else if (cache_size == 1) {
            /* only a(0) known; set a(1) */
            new_cache[1] = (seq_t)1;
            cache_size = 2;
        }

        /* compute missing terms using the recurrence */
        for (idx_t k = cache_size; k < new_size; ++k) {
            if ((k & 1) == 0) {               /* even index: minus */
                new_cache[k] = new_cache[k-1] - new_cache[k-2];
            } else {                           /* odd index: plus */
                new_cache[k] = new_cache[k-1] + new_cache[k-2];
            }
        }

        /* free old cache if we are in C++ */
#ifdef __cplusplus
        delete[] cache;
#endif
        /* attach new cache and update size */
        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
