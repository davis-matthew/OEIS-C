/*
 * A084386  Number of pairs of rabbits when there are 3 pairs per litter and
 *          offspring reach parenthood after 3 gestation periods; a(n) =
 *          a(n-1) + 3*a(n-3), with a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A084386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3), a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    static seq_t *cache = NULL;
    static idx_t  last   = 2;   /* last index that has a valid value */
    static idx_t  cap    = 0;   /* current capacity of cache array */

    /* Allocate cache if it does not exist yet */
    if (cache == NULL)
    {
        cap = (idx_t)(n + 1);
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)1;           /* out‑of‑memory fallback */

        cache[0] = cache[1] = cache[2] = (seq_t)1;  /* base values */
        last = 2;
    }
    /* Grow the cache if necessary */
    else if (n >= cap)
    {
        idx_t new_cap = cap ? cap * 2 : (idx_t)(n + 1);
        while (new_cap <= n)
            new_cap *= 2;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp)           /* only use new buffer if reallocation succeeds */
        {
            cache = tmp;
            cap = new_cap;
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = cache[i-1] + (seq_t)3 * cache[i-3];

    last = n;
    return cache[n];
}
