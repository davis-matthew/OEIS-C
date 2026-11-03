/*
 * A067691  a(n) = (a(n-1) + 1)^(n-1) for n > 0, a(0) = 0.
 * https://oeis.org/A067691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+1)^(n-1) for n>0, a(0)=0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
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
    static size_t cap   = 0;

    /* Ensure the cache can hold index n   */
    ensure_cache(n, &cache, &cap);

    /* Base case */
    if (n == 0)
    {
        cache[0] = 0;
        return 0;
    }

    /* If already computed, return it */
    if (cache[n] != 0)
        return cache[n];

    /* Compute all terms up to n, using the recurrence */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (cache[i] == 0)   /* not yet computed */
        {
            seq_t prev   = cache[i - 1];
            seq_t base   = prev + 1;
            seq_t exp    = (seq_t)(i - 1);
            cache[i]     = ipow(base, exp);
        }
    }

    return cache[n];
}
