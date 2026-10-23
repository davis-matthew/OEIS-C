/*
 * A039314  Numbers whose base-7 representation has the same nonzero number of
 *          1's and 2's.
 * https://oeis.org/A039314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-7 representation has the same nonzero number of 1's and 2's.
   Search implementation. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t next_scan = 1;          /* next integer to test */
    static seq_t *cache   = NULL;        /* cached sequence values */
    static size_t cache_len = 0;         /* number of cached terms */
    static size_t cache_cap = 0;         /* capacity of the cache */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        idx_t x = next_scan;
        int c1 = 0, c2 = 0;
        idx_t t = x;

        /* Count digits 1 and 2 in base 7. */
        while (t > 0)
        {
            int d = t % 7;
            if (d == 1) c1++;
            else if (d == 2) c2++;
            t /= 7;
        }

        if (c1 == c2 && c1 > 0)
        {
            /* Resize cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure */
                    return (seq_t)0;
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)x;
        }

        next_scan = x + 1;
    }

    return cache[n - 1];
}
