/*
 * A090010  Permanent of (0,1)-matrix of size n X (n+d) with d=6 and n zeros
 *          not on a line.
 * https://oeis.org/A090010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*a(n-1) + (n-1)*a(n-2), a(1)=6, a(2)=43
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 as a safe placeholder. */
    if (n < 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++ build – use std::vector for memoisation. */
    static std::vector<seq_t> cache;
    static bool initialised = false;

    if (!initialised)
    {
        cache.resize(3);           /* indices 0,1,2 */
        cache[1] = (seq_t)6;       /* a(1) */
        cache[2] = (seq_t)43;      /* a(2) */
        initialised = true;
    }

    if (n >= cache.size())
    {
        idx_t old_size = cache.size();   /* current number of stored terms */
        cache.resize(n + 1);              /* extend to contain a(n) */
        for (idx_t i = old_size; i <= n; ++i)
        {
            cache[i] = ((seq_t)i + (seq_t)5) * cache[i - 1]
                    + ((seq_t)i - (seq_t)1) * cache[i - 2];
        }
    }

    return cache[n];
#else
    /* C build – use dynamically allocated array. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL)   /* first call – initialise cache */
    {
        cache_size = 3;                      /* indices 0,1,2 */
        cache      = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */

        memset(cache, 0, cache_size * sizeof(seq_t)); /* initialise to 0 */
        cache[1] = (seq_t)6;                  /* a(1) */
        cache[2] = (seq_t)43;                 /* a(2) */
    }

    if (n >= cache_size)
    {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)    /* realloc failed – clean up and return 0 */
        {
            free(cache);
            cache      = NULL;
            cache_size = 0;
            return (seq_t)0;
        }

        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i)
        {
            cache[i] = ((seq_t)i + (seq_t)5) * cache[i - 1]
                    + ((seq_t)i - (seq_t)1) * cache[i - 2];
        }
        cache_size = n + 1;
    }

    return cache[n];
#endif
}
