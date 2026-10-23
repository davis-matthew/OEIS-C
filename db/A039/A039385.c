/*
 * A039385  Numbers whose base-9 representation has the same nonzero number of
 *          4's and 7's.
 * https://oeis.org/A039385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose base-9 representation has the same nonzero number
 *            of 4's and 7's
 * Range:     u8 n <= 6, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_47
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_47.h"

seq_t a(idx_t n)
{
    /* NOTE: The offset is 1; a(1) is the first term. */
    if (n == 0)
        return 0;          /* undefined, but safe return */

    static seq_t *cache = NULL;        /* cache of found terms */
    static size_t cache_size = 0;      /* how many terms cached so far */
    static size_t cache_cap  = 0;      /* allocated size of cache */
    static seq_t last_checked = 1;     /* next integer to test */

    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend cache until it contains at least n terms */
    while (cache_size < n) {
        if (has_equal_47(last_checked)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1; /* double capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;
    }

    return cache[n - 1];
}
