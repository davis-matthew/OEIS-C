/*
 * A052040  Numbers whose square is zeroless.
 * https://oeis.org/A052040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1381, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_in_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: numbers whose square contains no 0 digit */
/* The function generates the sequence on demand, caching results for
 * future calls.  It is O(n) overall, where n is the highest index
 * requested. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_zero_in_square.h"

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t terms_found = 0;
static seq_t next_candidate = 1;

/* Ensure cache can store at least 'needed' elements */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_size)
        return;
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    while (new_size < needed)
        new_size *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    /* n is 1‑based index into the sequence */
    if (n == 0)
        return 0; /* undefined, but safe for caller */

    while (terms_found < n) {
        /* find next integer whose square has no zero digit */
        while (has_zero_in_square(next_candidate))
            ++next_candidate;
        ensure_capacity(terms_found + 1);
        cache[terms_found] = next_candidate;
        ++terms_found;
        ++next_candidate;
    }
    return cache[n - 1];
}
