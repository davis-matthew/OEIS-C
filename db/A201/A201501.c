/*
 * A201501  Number of n X 5 0..1 arrays with every row and column running
 *          average nondecreasing rightwards and downwards, and the number of
 *          instances of each value within one of each other.
 * https://oeis.org/A201501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/**
 * Empirical recurrence for A201501 (OEIS A201501).
 *
 * a(n) =  1*a(n-1) +  2*a(n-2) - 3*a(n-3) +  1*a(n-4) +  2*a(n-5)
 *        - 4*a(n-6) +  2*a(n-7) +  2*a(n-8) - 4*a(n-9) +  0*a(n-10)
 *        + 4*a(n-11) - 2*a(n-12) - 2*a(n-13) + 4*a(n-14) - 2*a(n-15)
 *        - 1*a(n-16) + 3*a(n-17) - 2*a(n-18) - 1*a(n-19) + 1*a(n-20)
 *
 * Initial values a(1) … a(20) are hard‑coded (see OEIS entry).
 *
 * The function memoises results in a static array so that calls in any
 * order are O(1) after the necessary terms have been computed.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdint.h>   /* int64_t */

#define INIT 20

seq_t a(idx_t n)
{
    /* initial values a(1) … a(20) */
    static const seq_t init_vals[INIT] = {
        (seq_t)2,    /* a(1)  */
        (seq_t)3,    /* a(2)  */
        (seq_t)12,   /* a(3)  */
        (seq_t)12,   /* a(4)  */
        (seq_t)40,   /* a(5)  */
        (seq_t)32,   /* a(6)  */
        (seq_t)98,   /* a(7)  */
        (seq_t)73,   /* a(8)  */
        (seq_t)204,  /* a(9)  */
        (seq_t)141,  /* a(10) */
        (seq_t)380,  /* a(11) */
        (seq_t)252,  /* a(12) */
        (seq_t)650,  /* a(13) */
        (seq_t)414,  /* a(14) */
        (seq_t)1042, /* a(15) */
        (seq_t)649,  /* a(16) */
        (seq_t)1590, /* a(17) */
        (seq_t)967,  /* a(18) */
        (seq_t)2330, /* a(19) */
        (seq_t)1394  /* a(20) */
    };

    /* coefficients for the recurrence, length 20 */
    static const int coeff[INIT] = {
        1,   2,  -3,  1,   2,  -4,   2,   2,  -4,   0,
        4,  -2,  -2,  4,  -2,  -1,   3,  -2,  -1,   1
    };

    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (number of slots) */
    static idx_t cache_n   = 0;   /* number of values already computed */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = INIT;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – return 0 as sentinel */

        for (idx_t i = 0; i < INIT; ++i)
            cache[i] = init_vals[i];
        cache_n = INIT;
    }

    /* If requested term is already cached, return it */
    if (n <= cache_n)
        return cache[n - 1];

    /* Grow cache to accommodate terms up to n */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – return 0 as sentinel */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from cache_n+1 up to n */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        /* i is 1‑based; cache index is i‑1 */
        long long sum = 0;             /* accumulate in 64‑bit signed to avoid overflow */
        for (int k = 0; k < INIT; ++k) {
            /* a(i-k-1) is stored in cache[i-k-2] because array is 0‑based */
            sum += (long long)coeff[k] * (long long)cache[i - k - 2];
        }
        cache[i - 1] = (seq_t)sum;
    }

    cache_n = n;
    return cache[n - 1];
}
