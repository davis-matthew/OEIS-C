/*
 * A234484  Number of (n+1) X (2+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 28*a(n-2) + a(n-3) + 54*a(n-4) + 24*a(n-5)
 *            with initial values a(1)=242, a(2)=926, a(3)=3540, a(4)=13554,
 *            a(5)=51950
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static size_t cache_size = 0;

    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Lazily initialise the cache with the first 5 terms. */
    if (cache == NULL) {
        cache_size = 6;              /* indices 0..5 are valid */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 242;
        cache[2] = 926;
        cache[3] = 3540;
        cache[4] = 13554;
        cache[5] = 51950;
        max_n = 5;
    }

    /* Grow the cache if the requested index is out of bounds. */
    if ((idx_t)cache_size <= n) {
        size_t new_size = cache_size * 2;
        while ((idx_t)new_size <= n) new_size *= 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t t1 = 10 * cache[i - 1];
        seq_t t2 = 28 * cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = 54 * cache[i - 4];
        seq_t t5 = 24 * cache[i - 5];
        seq_t sum = t1 + t3 + t4 + t5;
        sum -= t2;
        cache[i] = sum;
    }
    if (n > max_n) max_n = n;

    return cache[n];
}
