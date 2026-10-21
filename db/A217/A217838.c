/*
 * A217838  Number of n element 0..1 arrays with each element the minimum of 7
 *          adjacent elements of a random 0..1 array of n+6 elements.
 * https://oeis.org/A217838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-8) Initial values: a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=7, a(4)=11, a(5)=16, a(6)=22, a(7)=29
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 65, u64 n <= 134,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial values */
    static const seq_t init_vals[8] = {1, 2, 4, 7, 11, 16, 22, 29};
    const idx_t init_n = 8;

    if (!cache) {
        cache_size = init_n;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < init_n; ++i)
            cache[i] = init_vals[i];
    }

    if (n < cache_size)
        return cache[n];

    /* extend cache to n+1 elements */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) /* allocation failed – returning 0 is harmless here */
        return 0;
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < init_n) {
            cache[i] = init_vals[i];
            continue;
        }
        seq_t v = cache[i - 1];
        v += v;               /* 2 * cache[i-1] */
        v -= cache[i - 2];
        v += cache[i - 8];
        cache[i] = v;
    }
    cache_size = new_size;

    return cache[n];
}
