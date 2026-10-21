/*
 * A073998  Number of strings of length n over GF(4) with trace 1 and subtrace
 *          1.
 * https://oeis.org/A073998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula : a(n) = 6*a(n-1) - 12*a(n-2) + 24*a(n-3) - 32*a(n-4)
 * Initial values : a(1)=0 , a(2)=2 , a(3)=7 , a(4)=16
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* greatest n for which cache[n] is valid */

    /* initialise cache with the first four values */
    if (cache == NULL)
    {
        size_t init_cap = 5;          /* indices 0..4 inclusive */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL)            /* fallback to zero if allocation fails */
            return 0;
        cache[0] = 0;            /* a(0) is unused but set to 0 for safety */
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 7;
        cache[4] = 16;
        max_cached = 4;
    }

    /* enlarge cache if the requested index is outside current bounds */
    while (n > max_cached)
    {
        idx_t new_cap = max_cached * 2;
        if (new_cap < n + 1) new_cap = n + 1;  /* ensure capacity for n */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                       /* keep old cache if realloc fails */
            break;   /* will return stale value for n if called again */
        cache = tmp;

        /* fill newly allocated part using the recurrence */
        for (idx_t i = max_cached + 1; i <= n; ++i)
        {
            cache[i] = ((seq_t)6) * cache[i - 1]
                     - ((seq_t)12) * cache[i - 2]
                     + ((seq_t)24) * cache[i - 3]
                     - ((seq_t)32) * cache[i - 4];
        }
        max_cached = n;
    }

    return cache[n];
}
