/*
 * A151573  a(0)=1, a(1)=1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A151573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1; a(2^i + j) = a(j) + 2*a(j+1) for 0 <= j <
 *            2^i
 * Range:     u8 n <= 59, u16 n <= 2040, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9289 (2026-11-03)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
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

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cache for computed values   */
    static size_t cap   = 0;        /* current capacity of cache   */
    static idx_t  max_computed = 0; /* highest index already computed */

    /* Ensure the cache is large enough. */
    ensure_cache(n, &cache, &cap);

    /* Base cases – they are safe to re‑assign on each call. */
    if (cap >= 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
    }

    /* If this is the first call, initialise the tracker. */
    if (max_computed < 1)
        max_computed = 1;

    /* Compute any missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Highest power of two that does not exceed i. */
        idx_t pow2 = ((idx_t)1) << ilog2(i);
        idx_t j = i - pow2;
        cache[i] = cache[j] + (seq_t)2 * cache[j + 1];
    }
    max_computed = n;

    return cache[n];
}
