/*
 * A099012  a(n) = 3^(n-1)*Fibonacci(n).
 * https://oeis.org/A099012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 3*a(n-1) + 9*a(n-2) for n≥2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cached_n = 0;   /* largest n for which cache[n] is valid */

    /* first call: allocate space for at least indices 0 and 1 */
    if (cache == NULL)
    {
        /* need room for index 0 (and possibly 1) */
        size_t sz = (n >= 1) ? (size_t)(n + 1) : 2;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL)
            return 0;          /* allocation failed – return 0 as safety */

        cache[0] = 0;
        cache[1] = 1;          /* always initialise 1 even if not needed yet */

        /* compute any values up to n */
        if (n >= 2)
        {
            for (idx_t i = 2; i <= n; ++i)
                cache[i] = 3 * cache[i - 1] + 9 * cache[i - 2];
        }
        cached_n = (n >= 1) ? n : 1;  /* at least 1 is cached */
    }
    /* subsequent calls: extend cache if needed */
    else if (n > cached_n)
    {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL)
            return cache[n];   /* allocation failed – return existing value */
        cache = tmp;
        /* compute new entries */
        for (idx_t i = cached_n + 1; i <= n; ++i)
            cache[i] = 3 * cache[i - 1] + 9 * cache[i - 2];
        cached_n = n;
    }
    /* return the requested value */
    return cache[n];
}
