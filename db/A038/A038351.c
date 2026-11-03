/*
 * A038351  Numbers whose base-6 representation has the same nonzero number of
 *          1's and 5's.
 * https://oeis.org/A038351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038351: Numbers whose base‑6 representation has the same *non‑zero*
   number of 1's and 5's.  Returns the nth such number (a(1) = 11). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true iff x contains the same positive number of 1's and 5's
   in its base‑6 representation. */
static bool has_equal_nonzero_ones_fives(seq_t x)
{
    seq_t t = x;
    int c1 = 0, c5 = 0;
    while (t != 0) {
        seq_t d = t % 6;
        if (d == (seq_t)1) ++c1;
        else if (d == (seq_t)5) ++c5;
        t /= 6;
    }
    return (c1 == c5) && (c1 > 0);
}

seq_t a(idx_t n)
{
    /* static state for caching the terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_capacity = 0;
    static seq_t candidate = 1;   /* next integer to test */

    /* initialise cache on first call */
    if (!cache) {
        cache_capacity = 16;
        cache = mem_new(seq_t, cache_capacity);
    }

    /* quick response if we already know the answer */
    if (n <= cache_size)
        return cache[n - 1];

    /* extend the cache until it contains at least n terms */
    while (cache_size < n) {
        if (has_equal_nonzero_ones_fives(candidate)) {
            /* grow cache if necessary */
            if (cache_size >= cache_capacity) {
                cache_capacity <<= 1;
                cache = mem_resize(seq_t, cache, cache_capacity);
            }
            cache[cache_size++] = candidate;
        }
        ++candidate;               /* examine the next integer */
    }

    return cache[n - 1];
}
