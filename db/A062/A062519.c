/*
 * A062519  Numbers for which (product of digits) / (sum of digits) is an
 *          integer > 1.
 * https://oeis.org/A062519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th element of OEIS A062519:
   numbers for which (product of digits)/(sum of digits) is an integer > 1. */
/* It keeps a static cache of found terms and expands it lazily. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached terms */
    static size_t len   = 0;        /* number of cached terms */
    static size_t cap   = 0;        /* current allocation size */
    static seq_t last   = (seq_t)0; /* last integer tested */

    /* Grow the cache until it contains at least n terms. */
    while (len < n) {
        /* Test the next integer. */
        last = last + (seq_t)1;
        seq_t v   = last;
        seq_t sum = 0;
        seq_t prod = 1;
        bool ok = true;

        seq_t t = v;
        while (t > (seq_t)0) {
            seq_t d = t % (seq_t)10;
            if (d == (seq_t)0) {    /* zero digit → product is 0 */
                ok = false;
                break;
            }
            sum  = sum + d;
            prod = prod * d;
            t    = t / (seq_t)10;
        }

        if (ok && prod % sum == (seq_t)0 && prod / sum > (seq_t)1) {
            /* Append to cache, enlarging it if necessary. */
            if (len == cap) {
                size_t new_cap = cap ? cap << 1 : 16;
                seq_t *new_cache;
                if (cache == NULL) {
                    new_cache = seq_alloc(new_cap);
                } else {
                    new_cache = seq_realloc(cache, cap, new_cap);
                }
                cache = new_cache;
                cap   = new_cap;
            }
            cache[len++] = v;
        }
    }

    return cache[n - (idx_t)1];
}
