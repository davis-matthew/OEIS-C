/*
 * A117312  Triangular numbers for which the sum of the digits is a heptagonal
 *          number.
 * https://oeis.org/A117312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117312: Triangular numbers whose digit sum is a heptagonal number.
   a(0) = 0 by definition.
   For n ≥ 1: compute the nth term by iterating over triangular numbers
   and selecting those whose digit sum is a heptagonal number.
   Memoisation is used so that any call order is O(1) after the one‑time
   construction. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/_floor_isqrt_u64.h"

/* Test whether x is a heptagonal number.
   An integer h is heptagonal iff 9 + 40·h is a perfect square s²
   and (3 + s) mod 10 == 0. */
static bool is_heptagonal(seq_t x)
{
    idx_t v   = 40 * (idx_t)x + 9;
    idx_t r   = _floor_isqrt_u64(v);
    return r * r == v && (3 + r) % 10 == 0;
}

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0, even though 0 is not chosen by the rule. */
    if (n == 0)
        return 0;

    /* Cache of found terms: cache[0] = a(1), cache[1] = a(2), … */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;      /* number of stored terms */
    static size_t cache_cap  = 0;      /* capacity of the cache */
    static idx_t  last_k     = 0;      /* next k to be tested */

    /* Build the cache until it contains at least n terms (for a(1)…a(n)). */
    while (cache_len <= (size_t)n - 1)
    {
        /* Grow cache capacity if necessary. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;        /* allocation failure → 0 */

            /* Copy existing data. */
            for (size_t i = 0; i < cache_len; ++i)
                tmp[i] = cache[i];

            free(cache);
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Generate the next triangular number t_k = k*(k+1)/2. */
        seq_t tri = ((seq_t)last_k * ((seq_t)last_k + 1U)) / 2U;

        /* Sum of its decimal digits. */
        seq_t t   = tri;
        seq_t sum = 0;
        while (t)
        {
            sum += t % 10U;
            t   /= 10U;
        }

        /* Store it if the sum is a heptagonal number. */
        if (is_heptagonal(sum))
            cache[cache_len++] = tri;

        ++last_k;
    }

    return cache[(size_t)n - 1];
}
