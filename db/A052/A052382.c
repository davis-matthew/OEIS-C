/*
 * A052382  Numbers without 0 in the decimal expansion, colloquial 'zeroless
 *          numbers'.
 * https://oeis.org/A052382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th positive integer whose decimal representation
 *            contains no digit 0 (zeroless numbers). This implementation
 *            generates the sequence on demand, caching all previously
 *            computed values. It works for any seq_t (8–128 bits or
 *            arbitrary‑precision integer) and for any idx_t >= 1. The
 *            runtim...
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  cached = 0;    /* number of terms cached so far */
    static size_t cap    = 0;    /* allocated capacity of cache   */
    static seq_t next_start = (seq_t)1; /* candidate to start new search */

    /* Ensure the cache can hold at least n terms */
    if (cached < n)
    {
        /* Grow the cache while needed */
        while (cached < n)
        {
            /* Find next candidate that has no zero digits */
            seq_t cur = next_start;
            while (true)
            {
                seq_t t = cur;
                bool zero = false;
                while (t > (seq_t)0)
                {
                    if (t % (seq_t)10 == (seq_t)0)
                    {
                        zero = true;
                        break;
                    }
                    t /= (seq_t)10;
                }
                if (!zero)
                    break;          /* cur is the next zeroless number */
                cur++;               /* skip numbers containing 0 */
            }

            /* Cache the found number */
            if (cached >= (idx_t)cap)
            {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc((void *)cache, newcap * sizeof(seq_t));
                if (!tmp)
                {
                    /* In a contest environment memory failure is unlikely;
                       terminate with a zero value.  */
                    return (seq_t)0;
                }
                cache = tmp;
                cap = newcap;
            }
            cache[cached] = cur;
            cached += 1;

            /* Prepare starting candidate for next iteration */
            next_start = cur + (seq_t)1;

            /* After the first term, next_start should always be cur+1,
               ensuring we start searching from the next integer. */
        }
    }

    /* Return the requested term */
    return cache[n - 1];
}
