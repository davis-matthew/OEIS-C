/*
 * A039436  Numbers whose base-10 representation has the same nonzero number
 *          of 7's and 9's.
 * https://oeis.org/A039436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A039436: numbers whose base‑10 representation
   has the same nonzero number of 7's and 9's.  The function enumerates
   such numbers in increasing order, caching them so that calls with
   arbitrary indices, not necessarily monotonic, are answered in
   O(1) after the first search for each additional term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of discovered terms   */
    static size_t cache_len = 0;      /* number of cached terms      */
    static size_t cache_cap = 0;      /* allocated capacity         */
    static idx_t cur = 0;             /* next integer to examine    */

    size_t target = (size_t)n;
    if (cache_len < target)
    {
        if (cache_cap == 0)
        {
            cache_cap = 8;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }

        while (cache_len < target)
        {
            cur++;                       /* examine next integer       */
            idx_t tmp = cur;
            unsigned int cnt7 = 0, cnt9 = 0;

            /* count digits 7 and 9 in tmp */
            while (tmp > 0)
            {
                unsigned int d = (unsigned int)(tmp % 10);
                if (d == 7) cnt7++;
                else if (d == 9) cnt9++;
                tmp /= 10;
            }
            if (cnt7 > 0 && cnt7 == cnt9)
            {
                if (cache_len == cache_cap)
                {
                    size_t newcap = cache_cap << 1;
                    seq_t *tmp_arr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp_arr) { /* allocation failed – return 0 as a sentinel */
                        free(cache);
                        cache = NULL;
                        cache_len = cache_cap = 0;
                        return (seq_t)0;
                    }
                    cache = tmp_arr;
                    cache_cap = newcap;
                }
                cache[cache_len++] = (seq_t)cur;
            }
        }
    }
    return cache[n - 1];
}
