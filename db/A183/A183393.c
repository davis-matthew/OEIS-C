/*
 * A183393  Half the number of n X 2 binary arrays with no element equal to a
 *          strict majority of its knight-move neighbors.
 * https://oeis.org/A183393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 6*a(n-3) - 6*a(n-5) + 3*a(n-6) -
 *            3*a(n-7) + a(n-8) for n>11 Initial values: a(1)=2, a(2)=8,
 *            a(3)=8, a(4)=8, a(5)=8, a(6)=8, a(7)=32, a(8)=128, a(9)=288,
 *            a(10)=648, a(11)=1800
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 49, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Multiply a seq_t by a small unsigned integer m (m <= 6) using repeated addition. */
static seq_t mul_small(seq_t x, unsigned int m) {
    seq_t result = (seq_t)0;
    for (unsigned int i = 0; i < m; ++i) result += x;
    return result;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index already filled */
    static size_t cache_cap = 0;  /* allocated capacity   */

    /* Initialize cache with the first 11 terms */
    if (cache_len == 0) {
        size_t init_cap = 16;
        while (init_cap <= 11) init_cap <<= 1;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache_cap = init_cap;
        cache_len = 11;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)8;
        cache[5] = (seq_t)8;
        cache[6] = (seq_t)8;
        cache[7] = (seq_t)32;
        cache[8] = (seq_t)128;
        cache[9] = (seq_t)288;
        cache[10] = (seq_t)648;
        cache[11] = (seq_t)1800;
    }

    /* Grow the cache if necessary */
    if (n > cache_len) {
        size_t required = (size_t)n + 1;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < required) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t t1 = mul_small(cache[i-1], 3);
            t1 += mul_small(cache[i-3], 6);
            t1 += mul_small(cache[i-6], 3);
            t1 += cache[i-8];

            seq_t t2 = mul_small(cache[i-2], 3);
            t2 += mul_small(cache[i-5], 6);
            t2 += mul_small(cache[i-7], 3);

            cache[i] = t1 - t2;
        }
        cache_len = n;
    }

    return cache[n];
}
