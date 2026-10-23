/*
 * A155459  a(n) = 7*a(n-1)+36*a(n-2), n>2 ; a(0)=1, a(1)=1, a(2)=7 .
 * https://oeis.org/A155459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)+36*a(n-2) (a(0)=1, a(1)=1, a(2)=7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 3, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_size = 0;  /* number of values already computed */

    /* --- ensure we can store values up to index n --- */
    size_t target = (size_t)n;     /* safe cast, n <= UINT64_MAX */

    /* Grow the cache if necessary */
    if (target >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
        while (target >= new_cap) new_cap <<= 1;

        /* Allocate a new buffer suitable for the target sequence type */
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!new_cache) abort();   /* OOM, unlikely in tests */
        /* Copy existing data to the new buffer */
        for (size_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old buffer if it exists (C++: delete[]) */
#ifdef __cplusplus
        if (cache) delete[] cache;
#else
        /* realloc already freed old block if new_cache != cache */
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* --- initialise the first three elements if not already done --- */
    if (cache_size < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache_size = 3;
    }

    /* --- compute values up to the requested index --- */
    if (cache_size <= target) {
        for (size_t i = cache_size; i <= target; ++i) {
            cache[i] = (seq_t)7 * cache[i-1] + (seq_t)36 * cache[i-2];
        }
        cache_size = target + 1;
    }

    return cache[target];
}
