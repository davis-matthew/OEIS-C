/*
 * A080881  a(n)*a(n+3) - a(n+1)*a(n+2) = 2^n, given a(0)=1, a(1)=2, a(2)=10.
 * https://oeis.org/A080881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 11*a(n+2) - 4*a(n), a(0)=1, a(1)=2, a(2)=10, a(3)=21
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 928 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms already computed */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* make sure the cache has room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)          cache[(size_t)i] = (seq_t)1;
        else if (i == 1)     cache[(size_t)i] = (seq_t)2;
        else if (i == 2)     cache[(size_t)i] = (seq_t)10;
        else if (i == 3)     cache[(size_t)i] = (seq_t)21;
        else                 cache[(size_t)i] = cache[(size_t)(i-2)] * (seq_t)11
                                            - cache[(size_t)(i-4)] * (seq_t)4;
    }

    /* update length of computed terms */
    if ((size_t)(n + 1) > cache_len) cache_len = (size_t)(n + 1);

    return cache[(size_t)n];
}
