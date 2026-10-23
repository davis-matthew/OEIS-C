/*
 * A039568  Numbers whose base-5 representation has the same number of 0's,
 *          3's and 4's.
 * https://oeis.org/A039568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   has_equal_034
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039568: numbers whose base-5 representation has equal numbers of 0's, 3's, and 4's. */
/* The code searches integers in increasing order, caching found terms. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_034.h"

/* Return the nth term of A039568 (1‑based indexing). */
seq_t a(idx_t n)
{
    /* static state (shared by all calls) */
    static seq_t *cache = NULL;      /* array of found terms */
    static idx_t idx_next = 1;       /* next integer to test */
    static size_t len = 0;           /* number of cached terms */
    static size_t cap = 0;           /* current allocated capacity */

    /* Grow the cache until it contains at least n terms. */
    while (len < (size_t)n) {
        /* Find the next integer satisfying the property. */
        while (!has_equal_034(idx_next))
            ++idx_next;

        /* Resize the array if necessary. */
        if (len == cap) {
            size_t new_cap = cap ? (cap << 1) : 8;
            seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * new_cap);
            /* copy old values to the new array */
            for (size_t i = 0; i < len; ++i)
                tmp[i] = cache[i];
            /* free the old array */
            free(cache);
            cache = tmp;
            cap = new_cap;
        }

        /* Store the qualifying number */
        cache[len++] = (seq_t)idx_next;
        ++idx_next;
    }

    return cache[n - 1];
}
