/*
 * A039018  Numbers whose base-6 representation has the same number of 0's and
 *          3's.
 * https://oeis.org/A039018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th positive integer whose base‑6 representation has the
   same number of digits 0 and 3.
   The implementation keeps a cached list of found values, extending it
   as needed by a simple incremental search. */

#include <stdlib.h>
#include <stdbool.h>

/* Test if a value has equal numbers of 0's and 3's in its base‑6 form. */
static bool good(seq_t v)
{
    int zeros = 0, threes = 0;
    seq_t t = v;
    if (t == 0) {          /* representation of 0 is "0" */
        zeros = 1;
    }
    while (t > 0) {
        int d = (int)(t % 6);
        if (d == 0) ++zeros;
        else if (d == 3) ++threes;
        t /= 6;
    }
    return zeros == threes;
}

seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache   = NULL;  /* array of found terms          */
    static size_t  cap    = 0;     /* allocated capacity            */
    static size_t  len    = 0;     /* number of entries stored      */
    static idx_t   found  = 0;     /* how many terms found so far   */
    static seq_t  next     = 1;    /* next integer to test          */

    /* If we already have the desired term, return it. */
    if (found >= n)
        return cache[n - 1];

    /* Allocate the cache the first time. */
    if (!cache) {
        cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
    }

    /* Search until we have enough terms. */
    while (found < n) {
        if (good(next)) {
            /* Grow the cache if necessary. */
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cap);
            }
            cache[len++] = next;
            ++found;
        }
        ++next;
    }

    return cache[n - 1];
}
