/*
 * A192813  Coefficient of x in the reduction of the polynomial x^(2*n) + x^n
 *          + 1 by x^3 -> x + 1.
 * https://oeis.org/A192813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 81, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) - a(n-4) - a(n-6)
 * Initial values: a(1)=0, a(2)=1, a(3)=1, a(4)=3, a(5)=4, a(6)=6
 * (OEIS A192813)
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based cache (cache[0] unused) */
static idx_t cached_n = 0;     /* largest n for which cache is filled */

seq_t a(idx_t n)
{
    /* allocate or enlarge cache to hold values up to n */
    if (n > cached_n)
    {
        idx_t new_n = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure – terminate */
        cache = new_cache;

        /* Fill missing terms */
        for (idx_t i = cached_n + 1; i <= new_n; ++i)
        {
            if (i == 1)      cache[1] = (seq_t)0;
            else if (i == 2) cache[2] = (seq_t)1;
            else if (i == 3) cache[3] = (seq_t)1;
            else if (i == 4) cache[4] = (seq_t)3;
            else if (i == 5) cache[5] = (seq_t)4;
            else if (i == 6) cache[6] = (seq_t)6;
            else
            {
                seq_t tmp = cache[i - 1];
                tmp += cache[i - 1];            /* 2*a(i-1) */
                tmp -= cache[i - 4];         /* subtract a(i-4) */
                tmp -= cache[i - 6];         /* subtract a(i-6) */
                cache[i] = tmp;
            }
        }
        cached_n = new_n;
    }

    return cache[n];
}
