/*
 * A043908  Numbers k such that 3 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has34opposite
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer k such that the base‑6
 * representation of k contains the digits 3 and 4 adjacent in either
 * order, while the base‑6 representation of k+1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has34opposite.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cache of found terms */
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return (seq_t)0;    /* undefined; placeholder */

    /* Grow cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Start searching from one greater than the last found term,
         * or from 1 if none yet. */
        seq_t start = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1] + 1;
        seq_t k = start;

        while (1)
        {
            if (has34opposite(k) && !has34opposite(k + 1))
            {
                /* Ensure cache has room. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;   /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                /* Stop once we have enough terms. */
                if (cache_len >= (size_t)n) break;
            }
            ++k;
        }
    }

    return cache[(size_t)n - 1];
}
