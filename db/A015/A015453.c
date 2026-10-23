/*
 * A015453  Generalized Fibonacci numbers.
 * https://oeis.org/A015453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-2); a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* Initialise or extend the cache up to index n */
    if (cache == NULL) {
        /* First allocation: size n+1 */
        cache_len = n + 1;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = (seq_t)7 * cache[i-1] + cache[i-2];
    } else if (cache_len <= n) {
        /* Resize cache to include index n */
        idx_t old_len = cache_len;
        cache_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;

        for (idx_t i = old_len; i <= n; ++i)
            cache[i] = (seq_t)7 * cache[i-1] + cache[i-2];
    }

    return cache[n];
}
