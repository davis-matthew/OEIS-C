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
 * Helpers:   digit_product, floor_sqrt, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* External helper functions provided by the build environment. */
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

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
