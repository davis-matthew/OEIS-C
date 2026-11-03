/*
 * A164404  Number of binary strings of length n with no substrings equal to
 *          0010 or 1011.
 * https://oeis.org/A164404
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 497 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4)  (n ≥ 8)
   Base values: a(4)=14, a(5)=24, a(6)=41, a(7)=70; undefined for n<4. */

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
    /* No defined value for indices below the offset. */
    if (n < 4) {
        return 0;
    }

    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;   /* array of a(i) values */
    static size_t cache_cap  = 0;      /* allocated capacity (>= n+1) */
    static idx_t  cache_max  = 0;      /* largest index that has been computed */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 4) {
            cache[i] = 14;
        } else if (i == 5) {
            cache[i] = 24;
        } else if (i == 6) {
            cache[i] = 41;
        } else if (i == 7) {
            cache[i] = 70;
        } else { /* i >= 8 */
            seq_t val = (seq_t)2 * cache[i - 1];
            val -= (seq_t)2 * cache[i - 3];
            val += (seq_t)2 * cache[i - 4];
            cache[i] = val;
        }
        cache_max = i;
    }

    return cache[n];
}
