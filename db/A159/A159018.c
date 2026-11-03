/*
 * A159018  a(0)=5; a(n) = a(n-1) + floor(sqrt(a(n-1))), n > 0.
 * https://oeis.org/A159018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 5; a(n) = a(n-1) + floor_sqrt(a(n-1)), n > 0
 * Range:     u8 n <= 14, u16 n <= 62, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8820 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size = 0;  /* number of terms currently stored */
    static idx_t cache_cap  = 0;  /* allocated capacity */

    /* initialisation: ensure capacity for the first term */
    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        /* sentinel: a(0) = 5 */
        cache[0] = (seq_t)5;
        cache_size = 1;
    }

    /* if n is already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow the cache until it holds up to n */
    if (n >= cache_cap) {
        /* double the capacity until it is enough */
        idx_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) {
            /* allocation failure: abort gracefully */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t inc  = floor_sqrt(prev);
        cache[i] = prev + inc;
    }
    cache_size = n + 1;

    return cache[n];
}
