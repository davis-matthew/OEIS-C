/*
 * A046173  Indices of square numbers that are also pentagonal.
 * https://oeis.org/A046173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) - a(n-2), a(1) = 1, a(2) = 99 (indices of
 *            square numbers that are also pentagonal)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[n-1] holds a(n) for n >= 1 */
static idx_t  cache_len = 0;  /* number of terms currently cached */

/* Return the nth term of A046173 (indices of square numbers that are also pentagonal). */
seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; undefined for 0, return 0 */
        return (seq_t)0;

    /* initialise cache with the first two terms if it hasn't been done yet */
    if (cache_len == 0)
    {
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (!cache)
            abort();                    /* out of memory */

        cache[0] = (seq_t)1;            /* a(1) */
        cache[1] = (seq_t)99;           /* a(2) */
        cache_len = 2;
    }

    /* extend the cache up to n if necessary */
    if (n > cache_len)
    {
        size_t newsize = (size_t)n * sizeof(seq_t);
        seq_t *tmp = (seq_t *)realloc(cache, newsize);
        if (!tmp)
            abort();                /* out of memory */
        cache = tmp;

        for (idx_t i = cache_len + 1; i <= n; ++i)
        {
            /* recurrence: a(i) = 98*a(i-1) - a(i-2) */
            cache[i - 1] = (seq_t)98 * cache[i - 2] - cache[i - 3];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
