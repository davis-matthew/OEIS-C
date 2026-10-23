/*
 * A181214  Number of n X 3 binary matrices with no three 1's adjacent in a
 *          line diagonally or antidiagonally.
 * https://oeis.org/A181214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 16*a(n-3) + 21*a(n-4) - 78*a(n-5) - 32*a(n-6)
 *            - 12*a(n-8) + 8*a(n-9) for n ≥ 10; initial values a(1)=8,
 *            a(2)=64, a(3)=400, a(4)=2500, a(5)=16100, a(6)=103684,
 *            a(7)=665252, a(8)=4268356, a(9)=27399292
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) {            /* under‐offset values are undefined */
        return 0;
    }

    /* Static cache for memoisation.  cache[i] holds a(i) for i>=1. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* number of allocated elements   */
    static idx_t  max_computed = 0;    /* highest index for which a() was computed */

    /* Initialisation on the first call. */
    if (!cache) {
        cache_size = 10;                  /* we fill indices 1 … 9, index 0 unused */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        static const seq_t base[10] = {
            0,                         /* dummy a(0) */
            8,                         /* a(1) */
            64,                        /* a(2) */
            400,                       /* a(3) */
            2500,                      /* a(4) */
            16100,                     /* a(5) */
            103684,                    /* a(6) */
            665252,                    /* a(7) */
            4268356,                   /* a(8) */
            27399292                   /* a(9) */
        };
        for (size_t i = 1; i < cache_size; ++i) {
            cache[i] = base[i];
        }
        max_computed = 9;
    }

    /* Grow the cache if the requested index exceeds current capacity. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size * 2;
        if ((size_t)n + 1 > new_size) {
            new_size = (size_t)n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                    /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            size_t idx = (size_t)i;
            /* For i >= 10 all required indices are defined. */
            seq_t pos =  6 * cache[idx - 1]
                       + 16 * cache[idx - 3]
                       + 21 * cache[idx - 4]
                       + 8  * cache[idx - 9];

            seq_t neg = 78 * cache[idx - 5]
                       + 32 * cache[idx - 6]
                       + 12 * cache[idx - 8];

            cache[idx] = pos - neg;
        }
        max_computed = n;
    }

    return cache[(size_t)n];
}
