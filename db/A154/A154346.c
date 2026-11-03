/*
 * A154346  a(n) = 12*a(n-1) - 28*a(n-2) for n > 1, with a(0)=1, a(1)=12.
 * https://oeis.org/A154346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 28*a(n-2) a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t curmax = 0;

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    /* ensure storage for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialise base values if necessary */
    if (curmax < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)12;
        curmax   = 1;
    }

    /* extend the table up to n */
    for (idx_t i = curmax + 1; i <= n; ++i) {
        cache[i] = (seq_t)12 * cache[i - 1] - (seq_t)28 * cache[i - 2];
    }
    curmax = n;

    return cache[n];
}
