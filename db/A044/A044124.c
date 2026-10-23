/*
 * A044124  Numbers k such that string 3,4 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has34_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044124
 * Numbers k such that the string "34" occurs in the base‑6 representation
 * of k but not of k-1.  The function searches for the next qualifying
 * integer and memoises results.  No simple closed formula is known,
 * so a straightforward search suffices within the required limits.
 */

#include <stdbool.h>
#include <stdlib.h>   /* for realloc and abort */
#include "../utils/has34_base6.h"

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* allocated capacity */
    static seq_t next_search = 1;   /* smallest candidate not yet examined */

    /* Grow the cache until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t k = next_search;

        /* Find the next k satisfying the condition. */
        while (true)
        {
            if (has34_base6(k) && !has34_base6(k - 1))
            {
                /* Ensure capacity */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();   /* memory exhaustion */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                next_search = k + 1;
                break;
            }
            k = k + 1;   /* test next candidate */
        }
    }

    return cache[n - 1];
}
