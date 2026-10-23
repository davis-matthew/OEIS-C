/*
 * A044002  Numbers k such that 0 and 7 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1298, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_juxtapose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044002: Numbers k such that 0 and 7 occur juxtaposed in the base-10 representation of k but not in k+1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_juxtapose.h"

/* Compute the nth term of A044002 (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms */
    static idx_t cached = 0;         /* number of cached terms */
    static size_t capacity = 0;      /* allocation capacity */

    if (n == 0) return 0;            /* undefined for n==0 */

    /* extend cache until it contains at least n terms */
    if (cached < n)
    {
        seq_t candidate = (cached == 0) ? 0 : cache[cached - 1] + 1;

        while (cached < n)
        {
            /* check if candidate matches the condition */
            if (has_juxtapose(candidate) && !has_juxtapose(candidate + 1))
            {
                /* grow array if necessary */
                if (cached + 1 > capacity)
                {
                    size_t new_cap = (capacity == 0) ? 64 : capacity * 2;
                    void *tmp = realloc(cache, sizeof(seq_t) * new_cap);
                    if (!tmp) exit(1);               /* out of memory */
                    cache = (seq_t *)tmp;
                    capacity = new_cap;
                }
                cache[cached++] = candidate;      /* store term */
            }
            candidate++;                          /* next integer to test */
        }
    }

    return cache[n - 1];
}
