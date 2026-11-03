/*
 * A138395  a(n) = 6*a(n-1) - 3*a(n-2), a(1) = 1, a(2) = 6.
 * https://oeis.org/A138395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2), a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 947 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; undefined for 0 */

    /* static cache: a[0] corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;  /* number of terms stored in cache */

    /* grow the cache if needed */
    if (n > cache_n) {
        /* reallocate to hold at least n terms */
        seq_t *new_cache = seq_realloc(cache, cache_n, n);
        if (!new_cache) return 0; /* allocation failure; caller can handle */
        cache = new_cache;

        /* initialize new entries */
        if (cache_n == 0) {
            cache[0] = (seq_t)1;          /* a(1) */
            if (n >= 2) cache[1] = (seq_t)6;  /* a(2) */
            cache_n = 2;
        } else if (cache_n == 1) {
            cache[1] = (seq_t)6;          /* a(2) */
            cache_n = 2;
        }

        /* compute remaining terms up to n */
        for (idx_t i = cache_n; i < n; ++i) {
            seq_t term_prev = cache[i-1];            /* a(i) where i>=2 */
            seq_t term_prevprev = cache[i-2];        /* a(i-1) where i>=3 */
            cache[i] = (seq_t)6 * term_prev - (seq_t)3 * term_prevprev;
        }
        cache_n = n;
    }

    return cache[n-1];
}
