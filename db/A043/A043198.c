/*
 * A043198  Numbers k such that 2 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_26_62
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th positive integer k whose base‑9
   representation contains the substring “26” or “62” and whose
   predecessor k-1 does not contain either substring.  The sequence
   is obtained by searching integers in increasing order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_26_62.h"

/* Cache of previously computed terms. */
static seq_t *cache     = NULL;
static size_t cache_len = 0; /* number of terms stored */
static size_t cache_cap = 0; /* allocated capacity */
static idx_t  last_k    = 0; /* largest k that has been examined */

/* Return a(n), 1‑based index. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* only defined for n >= 1 */

    /* Extend cache if needed. */
    while (cache_len < n) {
        idx_t k = last_k + 1;
        bool hk  = has_adjacent_26_62((seq_t)k);
        bool hk1 = has_adjacent_26_62((seq_t)(k - 1));

        if (hk && !hk1) {
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap) ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }

        last_k = k;
    }

    return cache[n - 1];
}
