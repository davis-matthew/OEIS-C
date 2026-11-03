/*
 * A164395  Number of binary strings of length n with no substrings equal to
 *          0001 or 0101.
 * https://oeis.org/A164395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4) with a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=8
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 82, u128 n <= 166,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 451 (2026-11-03)
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
    static idx_t max_n = (idx_t)-1;

    if (!cache) {
        /* Allocate space for at least the first four terms */
        ensure_cache(4, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        max_n    = 3;
    }

    if (n > max_n) {
        /* Ensure the array is large enough for index n */
        ensure_cache(n + 1, &cache, &cache_cap);
        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i-1] - cache[i-2] + 2 * cache[i-3] - 2 * cache[i-4];
        }
        max_n = n;
    }

    return cache[n];
}
