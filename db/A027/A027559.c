/*
 * A027559  Number of 4-balanced strings of length n: let d(S)= #(1)'s in S -
 *          #(0)'s, then S is k-balanced if every substring T has -k<=d(T)<=k;
 *          here k=4.
 * https://oeis.org/A027559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4); Initial values:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 37, u64 n <= 77, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <new>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoization array */
    static size_t cache_cap = 0;         /* current capacity of cache   */
    static idx_t  computed = 0;          /* maximum index computed so far */

    /* first call – initialise cache with the 5 known terms */
    if (cache == NULL)
    {
        cache_cap = 5;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        /* base values of the recurrence */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 8;
        cache[4] = 16;
        computed = 4;   /* we have terms up to index 4 ready */
    }

    /* if the requested index is beyond the current cache capacity,
       enlarge the cache */
    if (n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        /* copy existing entries */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* free old memory */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms that are still missing */
    if (computed < n)
    {
        for (idx_t i = computed + 1; i <= n; ++i)
        {
            seq_t val = cache[i-1] + 4 * cache[i-2];
            val -= 3 * cache[i-3];
            val -= 3 * cache[i-4];
            cache[i] = val;
        }
        computed = n;
    }

    return cache[n];
}
