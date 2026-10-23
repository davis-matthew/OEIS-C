/*
 * A043234  Numbers k such that 2 and 4 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has24or42
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has24or42.h"

/* 
   OEIS A043234:  Numbers k such that 2 and 4 occur juxtaposed in the
   decimal representation of k but not of k-1.
   The function generates the sequence by scanning positive integers,
   testing for the pattern "24" or "42".  It caches previously found
   terms so that calls with arbitrary n remain fast.
*/

seq_t a(idx_t n)
{
    /* Cache of already generated terms:
       cache[i] holds a(i+1) because the sequence is 1‑indexed. */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity */
    static idx_t    cached = 0;  /* number of terms stored */
    static seq_t    curr   = 0;  /* last examined integer */

    /* Ensure we have enough capacity */
    if (n > cached && (cap < n)) {
        size_t newcap = cap ? cap * 2 : 128;
        if (newcap < n) newcap = n;
        seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        if (cache) {
            for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Generate terms until we reach the requested index */
    while (cached < n) {
        ++curr;
        if (has24or42(curr) && !has24or42(curr - (seq_t)1)) {
            cache[cached++] = curr;
        }
    }

    return cache[n - 1];
}
