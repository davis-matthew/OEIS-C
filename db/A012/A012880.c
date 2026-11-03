/*
 * A012880  a(n+3) = 5*a(n+2)-4*a(n+1)+a(n).
 * https://oeis.org/A012880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n) Initial values: a(0)=0,
 *            a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 979 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                      /* malloc */
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_max = 0;             /* highest index computed (>=2 after init) */

seq_t a(idx_t n) {
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* initialise the cache once */
    if (!cache) {
        cache_cap = 3;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        computed_max = 2;
    }

    /* ensure we have space for index n */
    if ((size_t)n + 1 > cache_cap) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* compute any missing values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i-1] - (seq_t)4 * cache[i-2] + cache[i-3];
    }
    computed_max = n;

    return cache[n];
}
