/*
 * A099213  a(n) = a(n-1)+a(n-2)+3a(n-3), with a(0)=a(1)=a(2)=1.
 * https://oeis.org/A099213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n‑1)+a(n‑2)+3*a(n‑3) with a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  The function memoises the values in a static array so that calls
    may arrive in any order. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* memoised terms */
    static idx_t cache_size = 0;        /* allocated slots */
    static idx_t computed_max = 0;      /* highest n computed so far */

    /* Base values for n = 0,1,2 */
    if (n < 3)
        return (seq_t)1;

    /* Allocate or grow the cache as needed */
    if (cache == NULL) {
        cache_size = n + 1;
        if (cache_size < 3)
            cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        computed_max = 2;
    } else if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* Compute missing terms */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (seq_t)3 * cache[i-3];
    }
    computed_max = n;

    return cache[n];
}
