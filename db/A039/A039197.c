/*
 * A039197  Numbers whose base-11 representation has the same number of 5's
 *          and 8's.
 * https://oeis.org/A039197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039197 — numbers whose base‑11 representation has equal number of 5's and 8's. Computed by linear search. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t   last = 0;  /* next integer to test */

    while (len < n)
    {
        /* Find next qualifying number */
        while (1)
        {
            seq_t val = (seq_t)last;
            idx_t count5 = 0, count8 = 0;
            seq_t tmp   = val;

            while (tmp > 0)
            {
                seq_t d = tmp % (seq_t)11;
                if (d == (seq_t)5) count5++;
                else if (d == (seq_t)8) count8++;
                tmp = tmp / (seq_t)11;
            }

            last++;  /* prepare next candidate for next iteration */

            if (count5 == count8)
            {
                if (len + 1 > cap)
                {
                    size_t newcap = cap ? cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    cap   = newcap;
                }
                cache[len++] = val;
                break;  /* move on to next needed term */
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
