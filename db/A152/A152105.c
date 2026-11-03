/*
 * A152105  a(n) = (10^n + 6^n)/2.
 * https://oeis.org/A152105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 6^n)/2 Recurrence: a(n) = 16*a(n-1) - 60*a(n-2),
 *            with a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Allocate and free using malloc/free when compiled as C. */
static seq_t *alloc_cache(idx_t n)
{
    return mem_new(seq_t, n);
}

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

/* Compute a(n) = (10^n + 6^n)/2 for any n ≥ 0.
   Uses the linear recurrence and memoises computed values
   in a static cache so subsequent calls are O(1). */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base values the first time. */
    cache[0] = 1;
    if (n >= 1) {
        cache[1] = 8;
    }

    /* Compute any needed values using the recurrence. */
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i) {
            /* a(i) = 16*a(i-1) - 60*a(i-2) */
            cache[i] = (seq_t)16 * cache[i-1] - (seq_t)60 * cache[i-2];
        }
    }

    return cache[n];
}
