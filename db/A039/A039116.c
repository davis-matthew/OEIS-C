/*
 * A039116  Numbers whose base-10 representation has the same number of 0's
 *          and 5's.
 * https://oeis.org/A039116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-10 representation has the same number of 0's
 *            and 5's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   equal_zero_five
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_zero_five.h"

/* Return the n-th positive integer whose decimal digits contain
   an equal number of 0's and 5's.  The OEIS offset is 1. */
seq_t a(idx_t n)
{
    /* static cache of already found numbers */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t cur = (seq_t)1;          /* next number to test */

    /* make sure we have enough space for new elements */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate new values until the cache contains at least n terms */
    while (cache_len < n) {
        while (!equal_zero_five(cur)) cur++;  /* skip non‑qualifying numbers */
        cache[cache_len++] = cur;            /* store it */
        cur++;                               /* next to test in future */
    }

    return cache[n - 1];
}
