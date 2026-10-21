/*
 * A235081  Number of (n+1) X (2+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 10*a(n-2) - 33*a(n-3) - 28*a(n-4) + 111*a(n-5)
 *            + 20*a(n-6) - 111*a(n-7) - 27*a(n-8) + 30*a(n-9) + 10*a(n-10)
 *            Initial terms: a(1)=90, a(2)=198, a(3)=418, a(4)=930, a(5)=2002,
 *            a(6)=4514, a(7)=9838, a(8)=22402, a(9)=49294, a(10)=113086
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial non‑positive index (not expected in OEIS usage) */
    if (n == 0) return 0;

    static seq_t *cache = NULL;     /* cache[1] holds a(1) */
    static idx_t cache_cap = 0;     /* allocated capacity (size) */
    static idx_t cache_size = 0;    /* highest index computed */

    /* Initial data for n <= 10 */
    static const seq_t init[10] = {
        90,    /* a(1) */
        198,   /* a(2) */
        418,   /* a(3) */
        930,   /* a(4) */
        2002,  /* a(5) */
        4514,  /* a(6) */
        9838,  /* a(7) */
        22402, /* a(8) */
        49294, /* a(9) */
        113086 /* a(10) */
    };

    /* Allocate cache if needed (at least capacity 10) */
    if (cache == NULL) {
        cache_cap = (n >= 10) ? (idx_t)(n + 1) : 11;  /* +1 for 1‑based index */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;  /* allocation failure: return 0 as sentinel */
        /* copy initial terms */
        for (idx_t i = 1; i <= 10; ++i) cache[i] = init[i - 1];
        if (n >= 10) cache_size = 10;
        else cache_size = n;  /* for n < 10 all terms are already stored */
    }

    /* Grow cache if n exceeds current capacity */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If we already have the value, return it */
    if (n <= cache_size) return cache[n];

    /* Compute values from the next uncomputed index up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 10) {
            /* Should not happen because cache_size >=10 after init */
            cache[i] = init[i - 1];
        } else {
            /* Recurrence uses indices i-1 ... i-10 */
            seq_t term = 3  * cache[i - 1]
                       + 10 * cache[i - 2]
                       - 33 * cache[i - 3]
                       - 28 * cache[i - 4]
                       + 111 * cache[i - 5]
                       + 20 * cache[i - 6]
                       - 111 * cache[i - 7]
                       - 27 * cache[i - 8]
                       + 30 * cache[i - 9]
                       + 10 * cache[i -10];
            cache[i] = term;
        }
    }

    cache_size = n;
    return cache[n];
}
