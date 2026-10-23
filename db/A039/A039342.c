/*
 * A039342  Numbers whose base-8 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A039342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_a_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A039342:
   Numbers whose base-8 representation has the same non-zero number
   of 2's and 3's.  The function builds terms lazily and caches
   them, so it works correctly for any order of calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_a_candidate.h"

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* capacity of cache array */
static seq_t next_candidate = 1;     /* next integer to test */

/* Append a new value to the cache, expanding it if needed */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main function requested by the specification */
seq_t a(idx_t n)
{
    /* Return cached result if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate terms until we reach the desired index */
    while (cache_len < (size_t)n) {
        if (is_a_candidate(next_candidate))
            append_cache(next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
