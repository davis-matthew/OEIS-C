/*
 * A085371  Non-Niven (or non-Harshad) numbers that are divisible by 3.
 * https://oeis.org/A085371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose digit sum is a multiple of
 *            3 but does not divide the number
 * Range:     u8 n <= 34, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The condition uses the standard decimal digit sum function _digit_sum. */

#include <stdlib.h>
#include "../utils/_digit_sum.h"

/* Static cache of found terms, growing as needed. */
static seq_t *cache     = NULL;   /* array of cached terms */
static size_t cache_len = 0;      /* number of terms currently cached */
static size_t cache_cap = 0;      /* allocated capacity of cache */

/* The number to test next, always one greater than the last candidate tested. */
static idx_t next_candidate = 1;

/* Greatest candidate value already tested. */
static idx_t tested_max = 0;

/* Find the nth term of the sequence. */
seq_t a(idx_t n) {
    /* Return from cache if already computed. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
    }

    /* Continue testing candidates until we have n terms. */
    for ( ; cache_len < n; ++next_candidate ) {
        seq_t m = (seq_t)next_candidate;
        seq_t s = _digit_sum(m);

        /* Skip numbers whose digit sum is not a multiple of 3. */
        if ((s % 3) != 0)
            continue;

        /* Check that the digit sum does NOT divide the number. */
        if (m % s == 0)
            continue;

        /* Store the qualifying number. */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = m;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
