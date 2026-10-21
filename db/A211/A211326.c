/*
 * A211326  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one, two or three distinct values.
 * https://oeis.org/A211326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence a(n) = 5*a(n-1) - a(n-2) - 29*a(n-3) + 33*a(n-4) +
 *            50*a(n-5) - 88*a(n-6) - 14*a(n-7) + 73*a(n-8) - 22*a(n-9) -
 *            10*a(n-10) + 4*a(n-11), for n >= 12 Initial values: a(1)=25,
 *            a(2)=63, a(3)=149, a(4)=357, a(5)=829, a(6)=1941, a(7)=4479,
 *            a(8)=10413, a(9)=24087, a(10)=56079, a(11)=130523. Th...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n=0, return 0 */

    /* Base values up to a(11) */
    static const seq_t base[12] = {
        0,          /* dummy for index 0 */
        25,         /* a(1) */
        63,         /* a(2) */
        149,        /* a(3) */
        357,        /* a(4) */
        829,        /* a(5) */
        1941,       /* a(6) */
        4479,       /* a(7) */
        10413,      /* a(8) */
        24087,      /* a(9) */
        56079,      /* a(10) */
        130523      /* a(11) */
    };

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index that has been computed */

    /* Allocate or grow the cache as necessary */
    if (cache_size < n) {
        size_t new_cap = (size_t)(n + 1);   /* +1 to use 1-based indexing */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) return 0;    /* allocation failed */
        } else if (cache_size + 1 < new_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;    /* reallocation failed */
            cache = tmp;
        }

        /* Compute terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 11) {
                cache[i] = base[i];
            } else {
                seq_t res = 0;
                /* Positive contributions */
                res += cache[i - 1] * 5;
                res += cache[i - 4] * 33;
                res += cache[i - 5] * 50;
                res += cache[i - 8] * 73;
                res += cache[i - 11] * 4;
                /* Negative contributions */
                res -= cache[i - 2];
                res -= cache[i - 3] * 29;
                res -= cache[i - 6] * 88;
                res -= cache[i - 7] * 14;
                res -= cache[i - 9] * 22;
                res -= cache[i - 10] * 10;
                cache[i] = res;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
