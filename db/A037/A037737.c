/*
 * A037737  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3,0.
 * https://oeis.org/A037737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + pattern[(n-1) mod 4], with a(1)=2 where
 *            pattern = {2, 1, 3, 0} (periodic of length 4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

static const seq_t pattern[4] = { (seq_t)2, (seq_t)1, (seq_t)3, (seq_t)0 };

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    static idx_t cache_cap      = 0;      /* capacity of cache array         */
    static idx_t cache_len      = 0;      /* number of terms currently cached*/
    static seq_t *cache         = NULL;   /* array of cached terms           */

    /* Grow cache if necessary */
    if (cache_cap < n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (tmp == NULL)                /* allocation failure – unlikely */
            return (seq_t)0;
        cache     = tmp;
        cache_cap = n;
    }

    /* Extend cache up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        if (i == 1)
        {
            cache[0] = pattern[0];
        }
        else
        {
            cache[i - 1] = 4 * cache[i - 2] + pattern[(i - 1) % 4];
        }
    }
    cache_len = n;

    return cache[n - 1];
}
