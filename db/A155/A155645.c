/*
 * A155645  a(n) = 7^n+6^n-1.
 * https://oeis.org/A155645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 6^n - 1 Recurrence used: a(0) = 1, a(1) = 12, a(n)
 *            = 13*a(n-1) - 42*a(n-2) - 30 (n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

extern seq_t a(idx_t n) {
    /* Handle the trivial base case first */
    if (n == 0) {
        return 1;
    }

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_computed = 0;   /* highest index computed so far */

    /* Ensure that the cache has enough space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first two terms */
    if (max_computed == 0) {
        cache[0] = 1;
        cache[1] = 12;
        max_computed = 1;
    }

    /* If the requested term is already computed, return it */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Compute missing terms using the recurrence relation */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val *= 13;
        seq_t t = cache[i - 2];
        t *= 42;
        val -= t;
        val -= 30;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
