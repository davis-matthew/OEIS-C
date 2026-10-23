/*
 * A160552  a(0)=0, a(1)=1; a(2^i+j) = 2*a(j) + a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A160552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n≥2, let i be the largest power of two ≤ n,
 *            j=n‑i, then a(n)=2*a(j)+a(j+1)
 * Range:     u8 n <= 123, u16 n <= 4090, u32 n <= 9241, u64 n <= 9241,
 *            u128 n <= 9241, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;         /* array pointer */
    static size_t cache_cap = 0;        /* allocated size */
    static size_t cache_last = 0;       /* index of last computed element (>=1) */

    /* Initialise the cache on the first call.  20000 is safely larger than the
       10000‑term bound required by the tests.  If a larger n is requested we
       grow the array on demand. */
    if (!cache) {
        size_t init_cap = 20000;
        if ((idx_t)init_cap < n) init_cap = (size_t)n;   /* guard against very large first n */
        cache = alloc_seq_array(init_cap + 1);   /* allocate indices 0 … init_cap */
        if (!cache) return 0;    /* allocation failure – return 0 as a safe value */
        cache[0] = 0;
        cache[1] = 1;
        cache_cap  = init_cap + 1;
        cache_last = 1;
    }

    /* Grow the cache if the requested n is outside the current allocation. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return 0;
        /* Copy existing terms – seq_t assignment copies objects. */
        for (size_t i = 0; i <= cache_last; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if ((size_t)n > cache_last) {
        for (size_t k = cache_last + 1; k <= (size_t)n; ++k) {
            /* largest power of two ≤ k */
            unsigned int shift = 63 - __builtin_clzll((uint64_t)k);
            uint64_t power = 1ull << shift;
            size_t j = k - power;                 /* 0 ≤ j < power */
            cache[k] = cache[j] * 2 + cache[j + 1];
        }
        cache_last = (size_t)n;
    }

    return cache[(size_t)n];
}
