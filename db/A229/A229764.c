/*
 * A229764  Nim sequence of MARK: the game on n counters in which the legal
 *          moves are to remove 1 counter or to halve the number of counters
 *          and round down.
 * https://oeis.org/A229764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{a(n‑1), a(floor(n/2))}, a(0) = 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 101 terms through n = 100 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    /* Initial allocation and baseline term */
    if (cache == NULL) {
        ensure_cache(n, &cache, &cache_cap);
        cache[0] = 0;
        cache_len = 1;
    }

    /* Extend the cache up to n if needed */
    if ((idx_t)cache_len <= n) {
        ensure_cache(n, &cache, &cache_cap);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t half = cache[i / 2];
            seq_t mex = 0;
            if (prev == mex || half == mex) mex = 1;
            if (prev == mex || half == mex) mex = 2;
            cache[i] = mex;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
