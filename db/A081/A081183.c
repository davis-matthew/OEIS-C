/*
 * A081183  6th binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset directly */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache      = NULL;     /* array of computed terms      */
    static size_t cache_len  = 0;        /* number of terms currently in cache (0 .. cache_len-1) */
    static size_t cache_cap  = 0;        /* allocated capacity of cache  */

    /* Ensure the cache can hold terms up to index n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;  /* double or start at 8 */
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)      /* allocation failure – undefined behaviour for this harness */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise cache if it is still empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        if (cache_cap > 1)
            cache[1] = (seq_t)1;
        cache_len = 2;                 /* we now have terms 0 and 1 stored */
    }

    /* Compute any missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i-1] * (seq_t)12 - cache[i-2] * (seq_t)34;
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;          /* we now have terms 0..n stored */

    return cache[n];
}
