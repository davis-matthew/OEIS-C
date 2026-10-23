/*
 * A039358  Numbers whose base-9 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth positive integer whose base‑9 representation has the
 *            same non‑zero number of 0’s and 2’s
 * Range:     u8 n <= 18, u16 n <= 7529, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of stored terms   */
    static seq_t next_start = 1;   /* next integer to test    */

    /* a(0) is undefined; return 0 to keep the function total */
    if (n == 0)
        return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Ensure enough memory for at least 64 more terms. */
        if (cache_cap < cache_len + 64)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < cache_len + 64)
                new_cap = cache_len + 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Find the next suitable integer. */
        while (1)
        {
            seq_t x = next_start++;   /* candidate number */

            unsigned int c0 = 0, c2 = 0;   /* counters for 0 and 2 */

            seq_t t = x;
            while (t > 0)
            {
                /* Extract a base‑9 digit.  The result of % 9 is guaranteed
                   to be between 0 and 8, so we can safely cast.              */
                unsigned int d = (unsigned int)(t % (seq_t)9);
                if (d == 0) c0++;
                else if (d == 2) c2++;
                t /= 9;
            }

            /* Require the same non‑zero count for 0 and 2. */
            if (c0 == c2 && c0 > 0)
            {
                cache[cache_len++] = x;   /* store the found term */
                break;                    /* move on to next term   */
            }
        }
    }

    return cache[n - 1];
}
