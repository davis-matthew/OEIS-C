/*
 * A044003  Numbers k such that 0 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_08
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that k contains adjacent digits 0 and 8 in either order,
   but k+1 does not.  The sequence is indexed starting with a(1)=80. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_adjacent_08.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t  len    = 0;
    static size_t  cap    = 0;
    static idx_t   next   = 0;       /* next number to test */

    /* If we already have the requested element, return it. */
    if (len >= n)
        return cache[n-1];

    /* Expand the cache until we have at least n elements. */
    while (len < n) {
        ++next; /* test next candidate */

        if (has_adjacent_08(next) && !has_adjacent_08(next + 1)) {
            /* Ensure sufficient capacity. */
            if (len == cap) {
                cap = cap ? (cap * 2) : 8;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)next;
        }
    }

    return cache[n-1];
}
