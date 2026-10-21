/*
 * A211478  Number of -1..1 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having one, two or three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(n) = 7*a(n-1) - 15*a(n-2) + a(n-3) + 33*a(n-4)
 *            - 28*a(n-5) - 12*a(n-6) + 16*a(n-7) + a(n-8) - 2*a(n-9) for n ≥
 *            10, with initial values a(1)=9, a(2)=23, a(3)=59, a(4)=145,
 *            a(5)=351, a(6)=835, a(7)=1971, a(8)=4625, a(9)=10831
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* static cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside the defined offset */

    /* lazy initialization of the first 9 terms */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – abort */
            abort();
        }
        cache[0] = (seq_t)9;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)59;
        cache[3] = (seq_t)145;
        cache[4] = (seq_t)351;
        cache[5] = (seq_t)835;
        cache[6] = (seq_t)1971;
        cache[7] = (seq_t)4625;
        cache[8] = (seq_t)10831;
    }

    /* if we already have the requested term, return it */
    if (n <= cache_len) {
        return cache[n-1];
    }

    /* grow the cache to size n */
    seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort */
        abort();
    }
    cache = new_cache;

    /* compute missing terms from cache_len to n-1 (0‑based) */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] =
            (seq_t)( (seq_t)7 * cache[i-1]
                  - (seq_t)15 * cache[i-2]
                  + (seq_t)1  * cache[i-3]
                  + (seq_t)33 * cache[i-4]
                  - (seq_t)28 * cache[i-5]
                  - (seq_t)12 * cache[i-6]
                  + (seq_t)16 * cache[i-7]
                  + (seq_t)1  * cache[i-8]
                  - (seq_t)2  * cache[i-9] );
    }

    cache_len = n;
    return cache[n-1];
}
