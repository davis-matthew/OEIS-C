/*
 * A016153  a(n) = (9^n-4^n)/5.
 * https://oeis.org/A016153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A016153: a(n) = (9^n - 4^n)/5,  a(0)=0, a(1)=1
 * Recurrence for efficiency: a(n) = 13*a(n-1) - 36*a(n-2)
 */
#include <stddef.h>
#include <stdbool.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0; /* number of values currently stored */
static size_t cache_cap = 0; /* current allocation capacity */

seq_t a(idx_t n)
{
    /* Handle trivial cases optimally */
    if (n == 0) return (seq_t)0;
    /* Initialize cache for first two terms if not done yet */
    if (cache_len == 0) {
        ensure_cache(2, &cache, &cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }
    /* Grow and fill cache up to requested n */
    if ((size_t)n >= cache_len) {
        ensure_cache((size_t)n + 1, &cache, &cache_cap);
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)13 - cache[i - 2] * (seq_t)36;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
