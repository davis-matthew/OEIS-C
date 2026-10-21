/*
 * A232516  Number of (1+1) X (n+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          diagonally or antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + a(n-3) + 2*a(n-4) + 2*a(n-5) ; Initial values:
 *            a(1)=4, a(2)=16, a(3)=16, a(4)=54, a(5)=74
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* largest n computed and stored */

    /* base value for offset below 1 is not used */
    if (n == 0)
        return (seq_t)0;

    /* initialise cache with known first 5 terms */
    if (cache_len == 0) {
        /* ensure room for indices 0..5 */
        size_t init_size = 6;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache)
            abort();  /* cannot continue */

        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)54;
        cache[5] = (seq_t)74;
        cache_len = 5;
    }

    /* if requested n is already computed, return it */
    if (n <= cache_len)
        return cache[n];

    /* extend the cache to size n+1 (indices 0..n) */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();  /* cannot continue */
    cache = tmp;

    /* compute new terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term = 3 * cache[i - 2]
                   + cache[i - 3]
                   + 2 * cache[i - 4]
                   + 2 * cache[i - 5];
        cache[i] = term;
    }

    cache_len = n;
    return cache[n];
}
