/*
 * A191014  a(n) = 10*a(n-1) + 2*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A191014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 946 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) + 2*a(n-2),  a(0)=0, a(1)=1 */

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
    static idx_t cache_last = 0; /* last index whose value is already computed */

    /* trivial cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* guarantee enough memory for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base values once */
    if (cache_last == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache_last = 1;
    }

    /* if requested value already computed, return it */
    if (n <= cache_last) {
        return cache[(size_t)n];
    }

    /* compute missing values up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t prev1 = cache[(size_t)(i - 1)];
        seq_t prev2 = cache[(size_t)(i - 2)];
        cache[(size_t)i] = 10 * prev1 + 2 * prev2;
    }
    cache_last = n;

    return cache[(size_t)n];
}
