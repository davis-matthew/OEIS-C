/*
 * A039066  Numbers whose base-8 representation has the same number of 2's and
 *          7's.
 * https://oeis.org/A039066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th nonnegative integer whose base-8 representation
 *            contains an equal number of digits 2 and 7. (a(1)=0)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   equal_two_seven
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/equal_two_seven.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t next_candidate = 0;

/* Ensure that cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    /* Grow the cache as needed. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size == cache_cap) {
        cache_cap = cache_cap * 3 / 2 + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Scan numbers sequentially until we have n terms. */
    while (cache_size < (size_t)n) {
        if (equal_two_seven(next_candidate))
            cache[cache_size++] = next_candidate;
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1, but return 0 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
