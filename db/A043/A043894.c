/*
 * A043894  Numbers k such that 2 and 4 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has24_or42
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043894:
   numbers k such that digits 2 and 4 appear adjacent (in any order) in the
   base‑5 representation of k but not in that of k+1.  The function
   scans k in increasing order, caching found terms for repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has24_or42.h"

seq_t a(idx_t n)
{
    /* guard: OEIS offset is 1 */
    if (n == 0)
        return 0;

    /* static cache of terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static size_t len   = 0;    /* number of cached terms */
    static seq_t last_k = 0;    /* largest k examined so far */

    /* grow the cache until it contains at least n terms */
    while (len < n) {
        seq_t k = last_k + 1;
        /* find the next k satisfying the property */
        while (true) {
            if (has24_or42(k) && !has24_or42(k + 1))
                break;
            k++;
        }

        /* extend the cache array if necessary */
        if (len + 1 > cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) {            /* allocation failed */
                free(cache);
                cache = NULL;
                cap   = len = 0;
                last_k = 0;
                return 0;
            }
            cache = tmp;
            cap   = newcap;
        }

        cache[len] = k;
        len++;
        last_k = k;
    }

    return cache[n - 1];
}
