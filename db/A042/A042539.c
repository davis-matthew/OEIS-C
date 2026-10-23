/*
 * A042539  Denominators of continued fraction convergents to sqrt(798).
 * https://oeis.org/A042539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for a(n): a(0)=1, a(1)=4, a(2)=225, a(3)=904
   a(n) = 226*a(n-2) - a(n-4)  for n >= 4
   (obtained from the generating function -(x^2-4x-1)/(x^4-226x^2+1) )
*/
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed values */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t last = 0;        /* highest index computed so far */
    static int initialized = 0;

    /* initialise cache with first four values */
    if (!initialized)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        assert(cache);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)225;
        cache[3] = (seq_t)904;
        last = 3;
        initialized = 1;
    }

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values up to n if necessary */
    if (n > last)
    {
        for (idx_t i = last + 1; i <= n; ++i)
        {
            seq_t val;
            if (i <= 3)          /* safety, but cache already contains these */
                val = cache[i];
            else
                val = (seq_t)226 * cache[i - 2] - cache[i - 4];
            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
