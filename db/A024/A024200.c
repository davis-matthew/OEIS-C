/*
 * A024200  a(0) = 1, a(1) = 0, a(n+1) = 2*a(n) + (2*n-1)^2*a(n-1).
 * https://oeis.org/A024200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 0, a(n+1) = 2*a(n) + (2*n-1)^2*a(n-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 402 (2026-11-03)
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_up_to = 1;          /* 0 and 1 already stored */

    /* initialisation on first call */
    if (cache == NULL) {
        /* ensure capacity for indices 0 and 1 */
        ensure_cache(1, &cache, &cache_cap);
        cache[0] = 1;
        cache[1] = 0;
        computed_up_to = 1;
    }

    /* extend cache if needed */
    if (n > computed_up_to) {
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            idx_t k = i - 1;                     /* small integer */
            seq_t c = (seq_t)((2 * k) - 1);      /* 2k-1 as seq_t */
            c = c * c;                          /* (2k-1)^2   */
            cache[i] = cache[i - 1] * 2 + cache[i - 2] * c;
        }
        computed_up_to = n;
    }
    return cache[n];
}
