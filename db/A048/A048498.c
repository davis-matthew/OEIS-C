/*
 * A048498  a(n) = 2^(n-1)*(5*n-8) + 5.
 * https://oeis.org/A048498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3), a(0)=1, a(1)=2, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 863 (2026-11-03)
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
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;

    ensure_cache(n, &cache, &cap);

    if (computed < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        computed = 2;
    }

    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            cache[i] = (seq_t)5*cache[i-1] - (seq_t)8*cache[i-2] + (seq_t)4*cache[i-3];
        }
        computed = n;
    }

    return cache[n];
}
