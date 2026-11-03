/*
 * A087935  Perrin sequence of order 5.
 * https://oeis.org/A087935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5), with a(0)=5, a(1)=a(2)=a(3)=0, a(4)=4
 * Range:     u8 n <= 35, u16 n <= 71, u32 n <= 143, u64 n <= 286,
 *            u128 n <= 573, big any n
 * Validated: standard, pass, 76 terms through n = 8771 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

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
    static idx_t last_computed = 0;

    if (cache == NULL) {
        cache = alloc_seq_array(5);
        cache_cap = 5;
        cache[0] = 5;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 4;
        last_computed = 4;
    }

    if (n <= 4) {
        return cache[n];
    }

    ensure_cache(n, &cache, &cache_cap);

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = cache[i-4] + cache[i-5];
    }

    last_computed = n;
    return cache[n];
}
