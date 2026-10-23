/*
 * A043201  Numbers k such that 3 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has34_or_43, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Numbers k such that the base‑9 representation of k
 * contains the digits 3 and 4 as adjacent digits (either order),
 * while the base‑9 representation of k‑1 does not.
 * The sequence is indexed from n = 1.  */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/has34_or_43.h"

seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of stored terms    */
    static size_t cache_cap = 0;   /* allocated capacity       */
    static seq_t last_checked = 0; /* last number examined     */

    /* Extend the cache until it contains n terms. */
    while (cache_len < (size_t)n) {
        seq_t cand = last_checked + 1;   /* next candidate to test */

        /* Search forward until a valid term is found. */
        while (true) {
            if (has34_or_43(cand) && !has34_or_43(cand - 1)) {
                /* Store the new term. Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) return (seq_t)0;   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
                break; /* current candidate added to cache */
            }
            cand++; /* try next number */
        }
        last_checked = cand;   /* keep progress for future calls */
    }
    return cache[n - 1];
}
