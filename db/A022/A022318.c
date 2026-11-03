/*
 * A022318  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 4.
 * https://oeis.org/A022318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 1, a(1) = 4
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 856 (2026-11-03)
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

    /* Ensure the cache is large enough to hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the first two values if they are not already present. */
    if (!cache[0]) cache[0] = 1;
    if (!cache[1]) cache[1] = 4;

    /* If this value is already computed, return it immediately. */
    if (n <= cache_cap && cache[n] != 0)
        return cache[n];

    /* Compute values iteratively using the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] == 0) {
            cache[i] = cache[i - 1] + cache[i - 2] + 1;
        }
    }

    return cache[n];
}
