/*
 * A153388  Second bisection of A153382.
 * https://oeis.org/A153388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3); Initial values: a(0)=8,
 *            a(1)=18, a(2)=48
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 934 (2026-11-03)
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
    static size_t   cap   = 0;     /* size of allocated array */
    static idx_t   computed = 0;   /* number of entries already computed */

    /* If we already have the answer, we can return immediately. */
    if (n < computed) return cache[n];

    /* Make sure the array can hold index n. */
    ensure_cache(n, &cache, &cap);

    /* Initialize base values if not yet present. */
    if (computed == 0) {
        /* set values up to min(2, n) */
        cache[0] = (seq_t)8;
        computed = 1;
        if (n >= 1) {
            cache[1] = (seq_t)18;
            computed = 2;
        }
        if (n >= 2) {
            cache[2] = (seq_t)48;
            computed = 3;
        }
    } else {
        /* If some of the first three terms were not computed before,
           compute the missing ones while staying within requested n. */
        if (n >= 1 && computed <= 1) {
            cache[1] = (seq_t)18;
            computed = 2;
        }
        if (n >= 2 && computed <= 2) {
            cache[2] = (seq_t)48;
            computed = 3;
        }
    }

    /* Compute remaining terms using the linear recurrence. */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1] - 4 * cache[i - 2] + cache[i - 3];
    }
    computed = n + 1;

    return cache[n];
}
