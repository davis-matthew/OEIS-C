/*
 * A181600  Expansion of 1/(1 - x - x^2 + x^8 - x^10).
 * https://oeis.org/A181600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-8) + a(n-10); a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=13, a(7)=21, a(8)=33,
 *            a(9)=53
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 94, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cached_len = 0;    /* number of terms currently cached (next free index) */

seq_t a(idx_t n)
{
    /* Expand cache if necessary. */
    if ((size_t)n >= cached_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;

        /* Compute missing terms in increasing order. */
        for (size_t i = cached_len; i <= (size_t)n; ++i)
        {
            if (i == 0)
            {
                cache[0] = (seq_t)1;
            }
            else
            {
                seq_t val = 0;
                if (i >= 1) val += cache[i - 1];
                if (i >= 2) val += cache[i - 2];
                if (i >= 8) val -= cache[i - 8];
                if (i >= 10) val += cache[i - 10];
                cache[i] = val;
            }
        }
        cached_len = new_len;
    }

    return cache[(size_t)n];
}
