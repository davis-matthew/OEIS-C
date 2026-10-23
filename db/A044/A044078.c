/*
 * A044078  Numbers k such that string 0,1 occurs in the base 5 representation
 *          of k but not of k-1.
 * https://oeis.org/A044078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th base‑5 number that contains the substring "01"
 *            while the previous number (k - 1) does not
 * Range:     u8 n <= 12, u16 n <= 2929, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has01_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has01_base5.h"

/* Return the n-th term of the sequence.  The sequence is generated incrementally
   and cached so that arbitrarily ordered calls are efficient. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, harmless guard */

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static seq_t  last_k     = 0;          /* last checked k */

    /* Generate values until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k = ++last_k;                /* candidate number */

        if (has01_base5(k) && !has01_base5(k - 1)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure – unlikely */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = k;         /* store the found term */
        }
    }

    return cache[n - 1];
}
