/*
 * A043171  Numbers k such that 3 and 5 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the base‑8
 *            representation of k contains the substring 35 or 53, and the
 *            base‑8 representation of k‑1 does not contain that substring
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_juxp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_juxp.h"

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_k = 1;       /* next integer to test */

    /* Ensure we have at least n terms cached. */
    while (cache_len < n) {
        /* Test next_k for the juxtaposition property. */
        if (has_juxp(next_k) && !has_juxp(next_k - 1)) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? 2 * cache_cap : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();           /* unable to allocate more memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
