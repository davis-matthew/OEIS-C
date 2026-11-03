/*
 * A182558  a(n) = a(n-1)*2 - floor(sqrt(a(n-2))).
 * https://oeis.org/A182558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 977 (2026-11-03)
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

/* Sequence: a(0)=0, a(1)=1, a(n)=a(n-1)*2 - floor(sqrt(a(n-2))) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Base values directly returned. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Initialise cache on the first call. */
    if (cache_cap == 0) {
        cache_cap = 2;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(EXIT_FAILURE);      /* fatal error on allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;
    }

    /* Convert index to size_t for array access.  n is small in test cases. */
    size_t idx = (size_t)n;

    /* Extend cache if necessary. */
    if (idx >= cache_size) {
        /* Double capacity until it covers the requested index. */
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= idx)
            new_cap <<= 1;            /* new_cap = new_cap * 2 */

        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;

        /* Compute all missing terms up to idx. */
        for (size_t i = cache_size; i <= idx; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t val   = prev * (seq_t)2 - floor_sqrt(prev2);
            cache[i] = val;
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
