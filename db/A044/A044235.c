/*
 * A044235  Numbers n such that string 6,0 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Sequence A044235 – numbers n for which the base‑8 representation
 *            contains the substring "60" but the representation of n−1 does
 *            not
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has60_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has60_seq_t.h"

/* Main sequence function.  Memoises past values so calls in any order are fast. */
seq_t a(idx_t n)
{
    /* Static state for the memoisation cache. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_checked = 0;     /* last number examined (n-1 on entry of loop) */
    static bool last_has60 = false;    /* has60(last_checked)                       */

    if (n == 0) return (seq_t)0;       /* undefined below offset – return zero */

    /* Grow cache until we have computed the n‑th term. */
    while (cache_len < n) {
        idx_t  num = last_checked + 1;
        bool cur_has = has60_seq_t((seq_t)num);

        if (cur_has && !last_has60) {  /* pattern starts at num */
            if (cache_len >= cache_cap) {
                /* double capacity, at least 32 entries */
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();    /* memory error */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)num;
        }

        last_has60 = cur_has;
        last_checked = num;
    }

    return cache[n - 1];
}
