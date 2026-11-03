/*
 * A117464  Triangular numbers for which the product of the digits is a
 *          square.
 * https://oeis.org/A117464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth triangular number T that has the property that
 *            digit_product(T) is a perfect square. T_k = k*(k+1)/2, k >= 0
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 47 (2026-11-01)
 * Helpers:   digit_product, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* External helper functions provided by the build environment. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/ipow_i32.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache for previously found triangular numbers
       satisfying the digit‑product square condition. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_k = 0;

    /* Grow the cache until it contains at least (n+1) terms. */
    while (cache_len <= n)
    {
        /* Compute the next triangular number T_k. */
        seq_t t = ((seq_t)next_k) * ((seq_t)next_k + 1) / 2;

        /* Determine if the product of its decimal digits is a square. */
        seq_t prod = digit_product(t);
        seq_t s = floor_sqrt(prod);
        if (s * s == prod)
        {
            /* Expand storage if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }
        ++next_k;
    }

    return cache[n];
}
