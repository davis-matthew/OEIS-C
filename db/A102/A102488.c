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
 * Validated: standard, pass, 71 terms through n = 8589 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102488: Numbers in base-12 representation that contain at least one
   digit ≥ 10 (i.e., non‑decimal digits).  Computed by scanning successive
   integers and caching the results. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: true iff the base‑12 representation of x uses a digit ≥ 10. */
static bool has_non_decimal(seq_t x)
{
    /* All 0‑9 are acceptable digits; we only reject 10 (A) and 11 (B). */
    if (x < 10) return false;
    while (x)
    {
        seq_t r = x % (seq_t)12;
        if (r >= 10) return true;
        x /= (seq_t)12;
    }
    return false;
}

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
            cache = mem_new(seq_t, cache_cap);
        }

        while (cache_size < n)
        {
            if (has_non_decimal(next_candidate))
            {
                if (cache_size == cache_cap)
                {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_size++] = next_candidate;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
