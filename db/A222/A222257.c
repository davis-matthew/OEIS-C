/*
 * A222257  Lexicographically earliest injective sequence of positive integers
 *          such that the sum of 6 consecutive terms is always divisible by 6.
 * https://oeis.org/A222257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6); * Initial values: a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=4, a(4)=5, a(5)=9
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static idx_t count = 0;       /* number of terms currently stored (cache[0..count-1]) */
    static idx_t cap = 0;         /* current allocation size of cache */

    /* One-time initialisation of the first six terms */
    if (cache == NULL)
    {
        /* allocate space for at least the first six terms */
        cap   = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* on allocation failure return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)5;
        cache[5] = (seq_t)9;
        count = 6;
    }

    /* If the requested index is beyond what we currently have, grow the cache */
    if (n >= count)
    {
        /* Ensure we have enough space: grow by a factor of two until we exceed n */
        idx_t new_cap = cap;
        if (new_cap == 0) new_cap = 6;     /* safety for pathological first call */
        while (new_cap <= n)
            new_cap <<= 1;                 /* double the capacity */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;  /* allocation failure -> 0 */
        cache = tmp;
        cap   = new_cap;

        /* Compute all missing terms up to n */
        for (idx_t i = count; i <= n; ++i)
        {
            cache[i] = (seq_t)2 * cache[i - 3] - cache[i - 6];
        }
        count = n + 1;
    }

    return cache[n];
}
