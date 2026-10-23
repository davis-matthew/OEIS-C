/*
 * A043181  Numbers k such that 0 and 2 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adj_0_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the nth integer k whose base‑9 representation contains
   either "02" or "20" but k−1’s base‑9 representation does not.  */
/* This implementation keeps a cache of all values found so far, which
   is updated when the requested index is larger than those already
   computed.  Complexity is linear in the value of the nth term.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj_0_2.h"

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* capacity in terms                       */
    static size_t tot = 0;          /* number of terms currently stored        */

    /* extend the cache if necessary */
    if (n > cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort to avoid undefined behaviour */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute new terms up to index n */
    seq_t k = tot ? cache[tot - 1] + 1 : 0;   /* start after last known k */
    while (tot < (size_t)n) {
        if (has_adj_0_2(k) && !(k > 0 && has_adj_0_2(k - 1))) {
            cache[tot++] = k;
        }
        ++k;
    }

    /* now cache[n-1] holds the nth term */
    return cache[n - 1];
}
