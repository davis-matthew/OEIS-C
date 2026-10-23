/*
 * A044408  Numbers n such that string 7,6 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has76
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044408: numbers n such that the decimal representation contains the
   substring 76 but n−1 does not.  The function builds a static cache of
   found terms and returns the n‑th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has76.h"

/* Main sequence function with external linkage. */
seq_t a(idx_t n)
{
    /* Static cache holds already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;

    /* The last examined integer. */
    static seq_t cur = (seq_t)0;

    /* Minimum initial capacity. */
    if (cache_cap == 0) {
        cache_cap = 256;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory – undefined */
    }

    /* Grow the cache if necessary. */
    if (cache_sz + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* out of memory – undefined */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        /* Avoid wrap‑around of unsigned cur. */
        if (cur == (seq_t)(-1))
            break;   /* cannot find more terms; stop here */

        ++cur;     /* consider next integer */

        if (has76(cur)) {
            /* Check that cur-1 does NOT contain “76”. */
            bool prev_has = false;
            if (cur != (seq_t)0) {
                seq_t prev = cur - (seq_t)1;
                prev_has = has76(prev);
            }
            if (!prev_has) {
                cache[cache_sz++] = cur;   /* this is a new term */
            }
        }
    }

    /* If we didn't find enough terms (e.g., cur overflowed), return 0. */
    if (cache_sz < n)
        return (seq_t)0;

    return cache[n - 1];
}
