/*
 * A220078  a(n) = 5^(n!).
 * https://oeis.org/A220078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
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
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < max_computed)
        return cache[n];

    /* Make sure the cache can hold terms up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms sequentially */
    for (idx_t k = max_computed; k <= n; ++k)
    {
        if (k == 0)
        {
            /* Base case: a(0) = 5 */
            cache[0] = (seq_t)5;
        }
        else
        {
            /* Recurrence: a(k) = a(k-1)^k */
            cache[k] = ipow_idx(cache[k - 1], k);
        }
    }

    /* Update the number of terms that have been computed */
    max_computed = n + 1;
    return cache[n];
}
