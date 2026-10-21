/*
 * A205257  Number of (n+2) X 3 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally, and new values 0..1 introduced in row major
 *          order.
 * https://oeis.org/A205257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)+7*a(n-2)+32*a(n-3)+34*a(n-4)+14*a(n-5) *
 *            -20*a(n-6)-16*a(n-7) (n ≥ 8) * Initial values: * a(1)=240
 *            a(2)=1814 a(3)=13728 a(4)=103876 a(5)=785920 * a(6)=5946280
 *            a(7)=44989988
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache, 0‑based: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid terms in cache */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* initial 7 terms */
    static const seq_t init[7] = {
        (seq_t)240ULL,
        (seq_t)1814ULL,
        (seq_t)13728ULL,
        (seq_t)103876ULL,
        (seq_t)785920ULL,
        (seq_t)5946280ULL,
        (seq_t)44989988ULL
    };

    /* lazy allocation */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* ensure cache can hold n terms */
    if (cache_size < (size_t)n) {
        /* enlarge cache if needed */
        if (cache_cap < (size_t)n) {
            cache_cap = (size_t)n * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* fill missing terms */
        for (size_t i = cache_size; i < (size_t)n; ++i) {
            if (i < 7) {
                cache[i] = init[i];
            } else {
                /* i corresponds to a(i+1) because cache is 0‑based */
                seq_t sum1 = 0, sum2 = 0;
                /* terms: a(i) ... a(i-6) */
                size_t idx = i; /* index of a(i+1) in cache */
                sum1 = 6 * cache[idx - 1]
                     + 7 * cache[idx - 2]
                     + 32 * cache[idx - 3]
                     + 34 * cache[idx - 4]
                     + 14 * cache[idx - 5];
                sum2 = 20 * cache[idx - 6]
                     + 16 * cache[idx - 7];
                cache[i] = sum1 - sum2;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
