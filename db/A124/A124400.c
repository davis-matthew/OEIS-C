/*
 * A124400  a(n) = a(n-1) + 3*a(n-2) - a(n-4), with a(0)=1, a(1)=1, a(2)=4,
 *          a(3)=7.
 * https://oeis.org/A124400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-4) Initial terms: a(0)=1, a(1)=1,
 *            a(2)=4, a(3)=7
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;
    static size_t cache_cap = 0;   /* size allocated for cache */
    static idx_t   cached_to = 0;  /* highest index already computed */
    static int     init_done = 0;

    /* initialise cache with first four terms */
    if (!init_done) {
        size_t init_cap = 4;   /* need indices 0..3 */
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;               /* out of memory – abort */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 7;
        cache_cap  = init_cap;
        cached_to  = 3;
        init_done = 1;
    }

    /* trivial case: within already computed range */
    if (n <= cached_to)
        return cache[n];

    /* grow the cache if necessary */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap;
        while ((idx_t)(new_cap) <= n)
            new_cap = new_cap ? new_cap * 2 : 4;   /* double, or set to 4 */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* out of memory – abort */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cached_to + 1; i <= n; ++i)
        cache[i] = cache[i-1] + 3 * cache[i-2] - cache[i-4];

    cached_to = n;
    return cache[n];
}
