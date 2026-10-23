/*
 * A043906  Numbers k such that 2 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adj_24
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k with the digits 2 and 4 occurring adjacent in the
 * base‑6 representation of k (in either order), but not in that of k+1.
 * This code lazily builds a cache of the sequence terms.            */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj_24.h"

/* Return the n‑th element of the sequence, for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                             /* offset handling: ignored */

    /* Static cache of already computed terms.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_count = 0;             /* number of terms stored */
    static seq_t next_candidate = 1;          /* integer to test next */

    /* Grow cache if needed. */
    if (cache_count < n)
    {
        size_t new_cap = cache_count ? cache_count * 2 : 16;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                          /* allocation failure */
        cache = tmp;

        /* Compute terms until we have at least n of them. */
        while (cache_count < n)
        {
            /* Find the next candidate that satisfies the definition. */
            while (true)
            {
                if (has_adj_24(next_candidate) &&
                    !has_adj_24(next_candidate + 1))
                {
                    cache[cache_count] = next_candidate;
                    cache_count++;
                    next_candidate++;           /* start search after this k */
                    break;
                }
                next_candidate++;
            }
        }
    }
    return cache[n - 1];
}
