/*
 * A184766  Number of (n+1) X 3 0..3 arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A184766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 19*a(n-2) - 792*a(n-3) + 1800*a(n-4) Initial
 *            values: a(1)=424, a(2)=8272, a(3)=108496, a(4)=1792144
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0) return (seq_t)0;            /* undefined before offset */

    if (cache_len == 0) {                   /* initialise cache */
        cache_len = 5;                      /* indices 0..4 used */
        cache     = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)424;
        cache[2] = (seq_t)8272;
        cache[3] = (seq_t)108496;
        cache[4] = (seq_t)1792144;
    }

    if (n >= cache_len) {
        /* grow the cache to hold up to index n */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return cache[n];  /* keep using old data if realloc fails */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 20 * cache[i-1];
            val -= 19 * cache[i-2];
            val -= 792 * cache[i-3];
            val += 1800 * cache[i-4];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
