/*
 * A070207  Expansion of (1-x-5*x^2)/(1-3*x-2*x^2-x^3).
 * https://oeis.org/A070207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+2*a(n-2)+a(n-3) with a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 1720 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    ensure_cache(n, &cache, &cache_cap);

    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] + 2 * cache[i - 2] + cache[i - 3];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
