/*
 * A062906  Numbers k such that k and its reversal are both multiples of 17.
 * https://oeis.org/A062906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev_is_multiple_of_17
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching multiples of 17 such that the decimal reversal
   is also a multiple of 17 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rev_is_multiple_of_17.h"

/* Return the n-th term of OEIS A062906. */
seq_t a(idx_t n)
{
    /* Cache of discovered terms. */
    static seq_t *cache      = NULL;   /* dynamic array of terms */
    static size_t cache_len  = 0;      /* number of cached terms */
    static size_t cache_cap  = 0;      /* capacity of the array */
    /* Next candidate multiple of 17 to test. */
    static seq_t next_k      = 17;     /* start with the first multiple of 17 */

    /* Ensure the cache is initialised. */
    if (cache == NULL) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Continue searching until we have at least n terms. */
    while (cache_len < n) {
        if (rev_is_multiple_of_17(next_k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_k;
        }
        next_k = next_k + 17;   /* next multiple of 17 */
    }

    return cache[n - 1];
}
