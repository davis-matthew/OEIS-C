/*
 * A039007  Numbers whose base-5 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑5 representation
 *            contains the same number of 0’s and 1’s
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_01.h"

/* Return the n‑th term of OEIS A039007. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */

    /* static cache of already discovered terms */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static idx_t   cache_count = 0;
    static idx_t   next_candidate = 1;   /* next integer to test */

    /* If the requested term is already known, return it. */
    if (n <= cache_count) return cache[n-1];

    /* Grow the cache until it contains the n‑th term. */
    while (cache_count < n) {
        if (has_equal_01(next_candidate)) {
            /* Ensure the array is large enough. */
            if (cache_count == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                if (new_cap < cache_count + 1) new_cap = cache_count + 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_count] = (seq_t)next_candidate;
            cache_count++;
        }
        next_candidate++;
    }

    return cache[n-1];
}
