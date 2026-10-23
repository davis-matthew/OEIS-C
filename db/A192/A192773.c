/*
 * A192773  Coefficient of x in the reduction of the n-th Fibonacci polynomial
 *          by x^3->x^2+2x+1.
 * https://oeis.org/A192773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+5*a(n-2)-a(n-3)-5*a(n-4)+a(n-5)+a(n-6); Initial
 *            values: a(1)=0, a(2)=1, a(3)=0, a(4)=4, a(5)=3, a(6)=18
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;
    static idx_t last_computed = 0;

    if (n == 0)
        return (seq_t)0;   /* safety for non‑positive indices */

    /* First allocation and initialization of base terms */
    if (cache == NULL)
    {
        idx_t sz = 7;            /* indices 0..6 (0 unused) */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache_max = sz;

        /* Base values */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)3;
        cache[6] = (seq_t)18;
        last_computed = 6;
    }

    if (n <= last_computed)
        return cache[n];

    /* Resize if required */
    if (cache_max <= n)
    {
        idx_t new_max = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_max * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_max = new_max;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t term = cache[i - 1]
                    + (seq_t)5 * cache[i - 2]
                    - cache[i - 3]
                    - (seq_t)5 * cache[i - 4]
                    + cache[i - 5]
                    + cache[i - 6];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
