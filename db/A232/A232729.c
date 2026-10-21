/*
 * A232729  Number of n-digit numbers that yield an n-digit number after
 *          Reverse and Add.
 * https://oeis.org/A232729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1)+10*a(n-2)-100*a(n-3) for n>4; Initial values:
 *            a(1)=4, a(2)=45, a(3)=405, a(4)=4095
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* protect against zero index (undefined, but return 0) */
    if (n < 1) return (seq_t)0;

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (maximum index + 1) */
    static size_t max_computed = 0; /* highest index computed so far */

    /* first call: allocate space for at least indices 0..4 */
    if (cache_size == 0)
    {
        cache_size = (size_t)((n > 4) ? (n + 1) : 5);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */

        /* initialise the base cases */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)45;
        cache[3] = (seq_t)405;
        cache[4] = (seq_t)4095;
        max_computed = 4;
    }

    /* enlarge the cache if needed */
    if ((size_t)n + 1 > cache_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        cache     = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i)
    {
        if (i <= 4)
        {
            /* already initialised */
            continue;
        }
        cache[i] = (seq_t)10 * cache[i-1]
                 + (seq_t)10 * cache[i-2]
                 - (seq_t)100 * cache[i-3];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
