/*
 * A016311  Expansion of 1/((1-2*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A016311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 56*a(n-2) + 2^n for n ≥ 2 with a(0)=1,
 *            a(1)=17
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 494 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"
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

    /* base cases */
    if (n == 0) return 1;
    if (n == 1) return 17;

    /* make sure we have enough space for all indices up to n */
    if (cache == NULL || cache_cap <= (size_t)n)
        ensure_cache(n, &cache, &cache_cap);

    /* initialise the first two values (they will be overwritten on later calls,
       but this guarantees a known starting point) */
    cache[0] = 1;
    cache[1] = 17;

    /* Compute the recurrence up to n, overwriting any previously stored
       values so we need not keep a separate "computed" flag. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = 15 * cache[i - 1];
        seq_t term2 = 56 * cache[i - 2];
        seq_t term3 = pow2(i);          /* 2^i */
        cache[i] = term1 + term3 - term2;
    }

    return cache[n];
}
