/*
 * A052529  Expansion of (1-x)^3/(1 - 4*x + 3*x^2 - x^3).
 * https://oeis.org/A052529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 931 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1, a(1) = 1, a(2) = 4, a(3) = 13
 * Recurrence for n ≥ 4: a(n) = 4*a(n-1) - 3*a(n-2) + a(n-3)
 */
#include <stddef.h>
#include <stdint.h>
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    /* Handle small n directly */
    if (n <= 3) {
        static seq_t base[4] = {1, 1, 4, 13};
        return base[n];
    }

    /* Ensure the cache is large enough */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialize the first four terms if they are not yet set */
    if (cache[0] == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 13;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)4 * cache[i - 1]
                     - (seq_t)3 * cache[i - 2]
                     + cache[i - 3];
        }
    }

    return cache[n];
}
