/*
 * A155644  a(n) = 11^n-5^n+1.
 * https://oeis.org/A155644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n - 5^n + 1 Recurrence: a(0) = 1, a(1) = 7, for n ≥ 2
 *            a(n) = 16*a(n-1) - 55*a(n-2) + 40
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static size_t cap = 0;        /* allocated size of cache */
    static size_t len = 0;        /* number of computed terms (cache[0..len-1]) */

    /* Extend the cache if the requested index is not yet computed. */
    if (len <= n)
    {
        size_t new_cap = cap ? cap * 2 : 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failure */
        cache = tmp;
        cap   = new_cap;

        while (len <= n)
        {
            if (len == 0)
                cache[0] = (seq_t)1;          /* a(0) */
            else if (len == 1)
                cache[1] = (seq_t)7;          /* a(1) */
            else
            {
                seq_t v1 = cache[len-1];
                seq_t v2 = cache[len-2];
                cache[len] = (seq_t)16 * v1
                           - (seq_t)55 * v2
                           + (seq_t)40;
            }
            ++len;
        }
    }

    return cache[n];
}
