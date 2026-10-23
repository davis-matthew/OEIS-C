/*
 * A008824  Expansion of (1+2*x^4+x^7)/((1-x)^2*(1-x^7)).
 * https://oeis.org/A008824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 478, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008824: a(n) = coefficient of x^n in (1+2*x^4+x^7)/((1-x)^2*(1-x^7))
   Recurrence (n ≥ 9):  a(n)=2*a(n-1)-a(n-2)+a(n-7)-2*a(n-8)+a(n-9)
   Base values (0 ≤ n ≤ 8): 1, 2, 3, 4, 7, 10, 13, 18, 23
   The function memoises the sequence in a static array to allow arbitrary
   order of calls.  No external libraries are required. */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Static cache, its capacity and the highest computed index */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t last_computed = (size_t)-1;

    /* Ensure the cache can hold the requested index */
    size_t target = (size_t)n;            /* idx_t → size_t (fits) */

    if (cache_cap <= target)
    {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* memory error – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values on the first call */
    if (last_computed == (size_t)-1)
    {
        if (cache_cap < 9)    /* need space for indices 0..8 */
        {
            size_t new_cap = 16;
            while (new_cap < 9) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)10;
        cache[6] = (seq_t)13;
        cache[7] = (seq_t)18;
        cache[8] = (seq_t)23;
        last_computed = 8;
    }

    /* If we already have the needed value, return it */
    if (target <= last_computed)
        return cache[target];

    /* Compute new terms up to target using the recurrence */
    for (size_t i = last_computed + 1; i <= target; ++i)
    {
        seq_t term;
        if (i <= 8)
        {
            term = cache[i];          /* safety, should not happen */
        }
        else
        {
            term =   (seq_t)2 * cache[i - 1]
                    - cache[i - 2]
                    + cache[i - 7]
                    - (seq_t)2 * cache[i - 8]
                    + cache[i - 9];
        }
        cache[i] = term;
    }

    last_computed = target;
    return cache[target];
}
