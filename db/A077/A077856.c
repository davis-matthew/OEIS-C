/*
 * A077856  Expansion of (1-x)^(-1)/(1-2*x+x^2+x^3).
 * https://oeis.org/A077856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-4); Initial values: a(0)=1,
 *            a(1)=3, a(2)=6, a(3)=9, a(4)=10
 * Range:     i8 n <= 11, i16 n <= 24, i32 n <= 55, i64 n <= 112,
 *            i128 n <= 227, big any n
 * Validated: standard, pass, 59 terms through n = 923 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    static const seq_t init[5] = {
        (seq_t)1, (seq_t)3, (seq_t)6, (seq_t)9, (seq_t)10
    };
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n < 5)
        return init[(size_t)n];

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n + (idx_t)1, &cache, &cache_cap);

    if (cache_len == 0) {
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache[4] = init[4];
        cache_len = 5;
    }

    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 - (seq_t)3 * cache[i-2]
                 + cache[i-4];
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
