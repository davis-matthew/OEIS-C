/*
 * A033490  a(n) = 2*a(n-1) + a(floor(n/2)), with a(1) = 1, a(2) = 2.
 * https://oeis.org/A033490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, * a(n) = 2*a(n-1) + a(floor(n/2)), n ≥ 3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 983 (2026-11-03)
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
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;

    if (n == 0)
        return (seq_t)0;            /* undefined for n = 0, but harmless */

    /* Make sure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Compute values that have not yet been stored */
    if (computed < n) {
        if (computed == 0) {
            cache[1] = (seq_t)1;
            computed = 1;
            if (n >= 2) {
                cache[2] = (seq_t)2;
                computed = 2;
            }
        }
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else {
                cache[i] = (seq_t)2 * cache[i - 1] + cache[i / 2];
            }
        }
        if (n > computed)
            computed = n;
    }

    return cache[n];
}
