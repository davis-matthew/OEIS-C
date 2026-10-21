/*
 * A065007  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 4.
 * https://oeis.org/A065007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A065007 – numbers n for which the periodic part of the continued
   fraction of sqrt(n) begins with 4.  Computed by a simple brute‑force search. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Return the n-th integer whose continued fraction for sqrt(n)
   has periodic part starting with 4.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_to_test = (seq_t)2;   /* next integer to examine */

    while (cache_len < n) {
        seq_t v = next_to_test;
        ++next_to_test;                     /* advance for next iteration */

        /* Skip perfect squares.  floor_sqrt returns an integer ≤ sqrt(v). */
        seq_t a0  = floor_sqrt(v);
        seq_t d1  = v - a0 * a0;
        if (d1 == (seq_t)0)                /* v is a perfect square */
            continue;

        /* First partial quotient of sqrt(v):
           a1 = floor(2*a0 / d1)   (since a1 = floor((a0 + m1)/d1) with m1 = a0). */
        seq_t two_a0 = a0 + a0;
        seq_t a1 = two_a0 / d1;
        if (a1 == (seq_t)4) {               /* desired property */
            if (cache_len == cache_cap) {   /* grow the cache */
                size_t new_cap = cache_cap * 2 + 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = v;          /* store the term */
        }
    }
    return cache[n - 1];                    /* 1‑based indexing */
}
