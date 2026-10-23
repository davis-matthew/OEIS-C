/*
 * A039014  Numbers whose base-5 representation has the same number of 2's and
 *          4's.
 * https://oeis.org/A039014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A039014: numbers whose base‑5 representation has the same number
   of digits 2 and 4.  The function searches integers in ascending
   order, caches the results, and returns the n‑th element (n ≥ 1). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* safety for invalid index */

    /* -------------  static cache  ---------------- */
    static seq_t *cache = NULL;              /* array of found terms      */
    static idx_t cache_size = 0;             /* how many terms stored     */
    static idx_t cache_cap   = 0;             /* capacity of cache array   */
    static seq_t next_candidate = 0;         /* candidate to test next    */

    /* -------------  grow cache as needed  -------- */
    while (cache_size < n)
    {
        seq_t x = next_candidate;
        unsigned c2 = 0, c4 = 0;

        /* count 2's and 4's in base‑5 representation */
        while (x > 0)
        {
            unsigned d = (unsigned)(x % 5);
            if (d == 2) ++c2;
            else if (d == 4) ++c4;
            x /= 5;
        }

        if (c2 == c4)                         /* property satisfied       */
        {
            if (cache_size == cache_cap)      /* grow array if full       */
            {
                idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc((void *)cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp)                    /* allocation failed        */
                    return 0;                /* safe fallback            */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }

        ++next_candidate;                    /* move to the next integer */
    }

    return cache[n - 1];
}
