/*
 * A159263  a(0)=181; for n > 0, a(n) = a(n-1) + floor(sqrt a(n-1)).
 * https://oeis.org/A159263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 48, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 65 terms through n = 4887 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 181,
   a(n) = a(n‑1) + floor_sqrt(a(n‑1))   for n > 0 */
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
    /* Cache of already computed terms. */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of terms stored             */
    static size_t cache_cap = 0;   /* allocated size of cache array      */

    /* First call: allocate cache and seed a(0). */
    if (cache == NULL) {
        cache_cap = 64;                    /* initial capacity                  */
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) abort();
        cache[0] = (seq_t)181;              /* a(0)                              */
        cache_len = 1;                      /* we already have the first term    */
    }

    /* Term already computed?  */
    if (n < cache_len) return cache[n];

    /* Grow cache if necessary to hold indices up to n. */
    if ((idx_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t delta = floor_sqrt(prev);   /* floor(sqrt(prev))                 */
        cache[i] = prev + delta;
    }

    cache_len = n + 1;
    return cache[n];
}
