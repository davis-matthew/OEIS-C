/*
 * A039083  Numbers whose base-9 representation has the same number of 0's and
 *          7's.
 * https://oeis.org/A039083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039083 – Numbers whose base-9 representation has the same number of 0’s and 7’s.
   The function enumerates natural numbers, keeps those satisfying the
   property, and returns the n-th such number.  It memoises the sequence
   so repeated calls are fast. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/good_base9.h"

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n)
{
    /* Static state kept between calls. */
    static seq_t *cache   = NULL;   /* array of found terms */
    static size_t  cap    = 0;      /* current array capacity */
    static idx_t   count  = 0;      /* number of terms stored */
    static seq_t   last_checked = 0;/* last number examined */

    /* Grow the cache to hold at least n terms. */
    if (cap < n) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {        /* allocation failure – give up */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (count < n) {
        last_checked = last_checked + 1U;
        if (good_base9(last_checked)) {
            cache[(size_t)count] = last_checked;
            count++;
        }
    }

    return cache[(size_t)(n - 1)];
}
