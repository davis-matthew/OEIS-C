/*
 * A102488  Numbers in base-12 representation that cannot be written with
 *          decimal digits.
 * https://oeis.org/A102488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_non_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102488: Numbers in base-12 representation that contain at least one
   digit ≥ 10 (i.e., non‑decimal digits).  Computed by scanning successive
   integers and caching the results. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_non_decimal.h"

/* Return the n‑th member of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Cache of completed terms.  cache[0] == a(1). */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;
    static seq_t next_candidate = 0;   /* next integer to test */

    /* Expand cache and find terms up to n. */
    if (cache_size < n)
    {
        if (cache_cap == 0)
        {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }

        while (cache_size < n)
        {
            if (has_non_decimal(next_candidate))
            {
                if (cache_size == cache_cap)
                {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = next_candidate;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
