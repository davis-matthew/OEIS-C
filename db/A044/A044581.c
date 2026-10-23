/*
 * A044581  Numbers k such that string 1,5 occurs in the base 8 representation
 *          of k but not of k+1.
 * https://oeis.org/A044581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044581: a(n) = k such that the base‑8 representation of k contains the
   substring "15" but the base‑8 representation of (k+1) does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has15.h"

seq_t a(idx_t n)
{
    /* Static cache to allow O(n) time with arbitrary order of calls. */
    static seq_t *cache     = NULL;
    static size_t  len      = 0;   /* number of terms currently stored */
    static size_t  cap      = 0;   /* allocated capacity of cache */
    static seq_t  next      = (seq_t)0;   /* last number examined */

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        cap   = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n entries. */
    while (len < n) {
        /* Grow capacity if necessary. */
        if (len + 1 > cap) {
            size_t new_cap = cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL)
                exit(EXIT_FAILURE);
            cache = new_cache;
            cap   = new_cap;
        }

        /* Check the next integer. */
        seq_t k = next + (seq_t)1;
        next = k;

        if (has15(k) && !has15(k + (seq_t)1))
            cache[len++] = k;
    }

    return cache[n - 1];
}
