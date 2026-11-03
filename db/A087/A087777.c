/*
 * A087777  a(1) = ... = a(4) = 1; a(n) = a(n - a(n-2)) + a(n - a(n-4)).
 * https://oeis.org/A087777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - a(n-2)) + a(n - a(n-4)), a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 401, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9853 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
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
static size_t cap = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* Undefined, but not used */

    /* Allocate/extend cache if necessary and initialize base values once */
    if (cache == NULL) {
        ensure_cache(n, &cache, &cap);
        /* Store the first four terms */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        max_computed = 4;
    } else if (n > max_computed) {
        /* Ensure we have room for up to n */
        ensure_cache(n, &cache, &cap);
    }

    /* If n is within the pre‑computed part, return it directly */
    if (n <= max_computed)
        return cache[n];

    /* Compute from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t idx1 = i - (idx_t)cache[i - 2];
        idx_t idx2 = i - (idx_t)cache[i - 4];
        /* idx1 and idx2 are guaranteed to be >= 1 and <= i-1 */
        cache[i] = cache[(size_t)idx1] + cache[(size_t)idx2];
    }
    max_computed = n;
    return cache[n];
}
