/*
 * A043158  Numbers k such that 0 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adj07or70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k such that the base‑8
   representation of k contains an adjacent pair of digits 0 and 7
   (in either order), but the representation of k‑1 does not.  The
   function is computed by online linear search with memoisation. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj07or70.h"

seq_t a(idx_t n)
{
    /* The sequence has offset 1; for n==0 return 0 (undefined) */
    if (n == 0) return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored in cache   */
    static size_t cache_cap = 0;   /* allocated capacity                */
    static seq_t next_check = (seq_t)1; /* next integer to examine       */

    /* Generate terms until we have at least the requested index */
    if ((size_t)n > cache_len) {
        /* Ensure the cache can hold at least n terms */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            /* realloc must be cast to seq_t* for C++ compatibility */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        while (cache_len < (size_t)n) {
            if (has_adj07or70(next_check) &&
                !has_adj07or70(next_check - (seq_t)1)) {
                cache[cache_len++] = next_check;
            }
            ++next_check;
        }
    }

    /* Return the (n‑1)th zero‑based index term */
    return cache[(size_t)(n - 1)];
}
