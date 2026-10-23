/*
 * A185859  1/128 the number of (n+2) X 4 binary arrays with no 3 X 3 subblock
 *          trace equal to any horizontal or vertical neighbor 3 X 3 subblock
 *          trace.
 * https://oeis.org/A185859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 17*a(n-2) - 36*a(n-3) + 24*a(n-4) - 6*a(n-5)
 *            a(1) = 22, a(2) = 164, a(3) = 1254, a(4) = 9604, a(5) = 73542
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* base values for n=1..5 */
    static const seq_t base[6] = {
        (seq_t)0,      /* unused index 0 */
        (seq_t)22,
        (seq_t)164,
        (seq_t)1254,
        (seq_t)9604,
        (seq_t)73542
    };

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;   /* highest index computed */
    static size_t cache_cap = 0;  /* allocated size (elements) */

    /* if n is within the known base values */
    if (n <= 5) {
        return base[(size_t)n];
    }

    /* allocate (or grow) the cache if necessary */
    if (cache == NULL) {
        /* initial allocation: at least up to n */
        size_t init_size = (n + 1 > 6) ? n + 1 : 6;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        for (size_t i = 1; i <= 5; ++i) {
            cache[i] = base[i];
        }
        cache_max = 5;
        cache_cap = init_size;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        } else {
            /* allocation failed; keep old cache */
        }
    }

    /* compute values up to n, using recurrence */
    while (cache_max < n) {
        idx_t idx = cache_max + 1;
        seq_t term = 0;
        term += cache[idx-1] * (seq_t)6;      /* 6*a(n-1) */
        term += cache[idx-2] * (seq_t)17;     /* 17*a(n-2) */
        term -= cache[idx-3] * (seq_t)36;     /* -36*a(n-3) */
        term += cache[idx-4] * (seq_t)24;     /* 24*a(n-4) */
        term -= cache[idx-5] * (seq_t)6;      /* -6*a(n-5) */
        cache[idx] = term;
        cache_max = idx;
    }

    return cache[(size_t)n];
}
