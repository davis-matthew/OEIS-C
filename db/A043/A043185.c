/*
 * A043185  Numbers k such that 0 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_adjacent_6_0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043185: Numbers k such that 0 and 6 occur juxtaposed in the base‑9
   representation of k but not of k-1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_6_0.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of terms cached */
    static size_t cap  = 0;   /* allocation capacity */
    static seq_t last_checked = (seq_t)0;   /* numbers examined up to this value */

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        /* Grow the cache if necessary before accessing it */
        if (len + 1 > cap) {
            size_t new_cap = (cap == 0) ? 64 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();       /* allocation failure: terminate */
            cache = tmp;
            cap   = new_cap;
        }

        seq_t k = last_checked + (seq_t)1;   /* next candidate */
        if (has_adjacent_6_0(k) && !has_adjacent_6_0(k - (seq_t)1)) {
            cache[len++] = k;   /* store found term */
        }
        last_checked = k;       /* update last examined number */
    }

    return cache[n - 1];
}
