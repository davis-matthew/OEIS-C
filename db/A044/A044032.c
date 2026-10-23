/*
 * A044032  Numbers k such that 5 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A search for numbers k that contain “57” or “75” in their
 *            decimal expansion while k+1 does not contain either of those
 *            adjacent digit pairs
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has57or75
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has57or75.h"

/* Cached state for incremental generation. */
static idx_t              computed_terms = 0;   /* how many terms have been stored */
static seq_t*             cache          = NULL; /* array of previously computed terms */
static size_t             cache_capacity = 0;   /* allocated size of cache   */
static seq_t              next_scan      = 57;   /* next integer to test       */

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1, so this value is undefined */
        return 0;

    /* Grow cache if necessary. */
    if (cache_capacity < (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t* new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)   /* allocation failed – abort */
            abort();
        cache          = new_cache;
        cache_capacity = new_cap;
    }

    /* Generate terms up to the requested index. */
    while (computed_terms < n) {
        seq_t x = next_scan;
        /* Search for the next qualifying number. */
        for (;;) {
            if (has57or75(x) && !has57or75(x + 1))
                break;
            ++x;
        }
        cache[computed_terms] = x;
        ++computed_terms;
        next_scan = x + 1;   /* start next search after the found term */
    }

    return cache[n - 1];
}
