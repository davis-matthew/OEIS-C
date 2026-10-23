/*
 * A043256  Numbers k such that 6 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adj68or86
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term definition: a(n) is the n-th positive integer k such that
   the decimal representation of k contains the sub‑string "68" or
   "86", and the decimal representation of k−1 does NOT contain that
   sub‑string.  The OEIS offset is 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj68or86.h"

seq_t a(idx_t n)          /* n ≥ 1 */
{
    /* Static cache of computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k     = 1;   /* next integer to test */

    /* Ensure cache holds at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next qualifying integer */
        while (1) {
            seq_t k = next_k++;
            if (has_adj68or86(k) && !has_adj68or86(k - (seq_t)1)) {
                /* Store k */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) { /* allocation failure – unwind to caller */
                        free(cache);
                        cache = NULL;
                        cache_len = cache_cap = 0;
                        return (seq_t)0; /* will not happen in built tests */
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
