/*
 * A039329  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 1's.
 * https://oeis.org/A039329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate natural numbers and select those whose base‑8 representation
   contains the same positive number of 0's and 1's. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* next integer to test */
    static seq_t next_candidate = 1;

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        /* search for the next term */
        for (;;)
        {
            seq_t m = next_candidate;
            ++next_candidate;

            /* count zeros and ones in base‑8 */
            idx_t c0 = 0, c1 = 0;
            seq_t t = m;
            while (t > (seq_t)0)
            {
                unsigned d = (unsigned)(t % (seq_t)8);
                if (d == 0)
                    ++c0;
                else if (d == 1)
                    ++c1;
                t /= (seq_t)8;
            }
            /* keep only if counts are equal and positive */
            if (c0 == c1 && c0 > 0)
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (tmp == NULL) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = m;
                break; /* found the next term, move to outer loop */
            }
        }
    }

    return cache[n - 1];
}
