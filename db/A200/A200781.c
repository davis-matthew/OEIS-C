/*
 * A200781  G.f.: 1/(1-5*x+10*x^3-5*x^4).
 * https://oeis.org/A200781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-3) + 5*a(n-4) (for n≥4) a(0)=1, a(1)=5,
 *            a(2)=25, a(3)=115
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static const seq_t init_vals[4] = {1, 5, 25, 115};

    /* initialise cache if needed */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
    }

    /* return known value if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow cache to accommodate n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (!tmp) abort();
    cache = tmp;
    /* compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 5*a(i-1) - 10*a(i-3) + 5*a(i-4) */
        seq_t sum = cache[i - 1] + cache[i - 4];   /* a(i-1) + a(i-4) */
        sum = sum * 5;                              /* 5*(a(i-1)+a(i-4)) */
        seq_t sub = cache[i - 3] * 10;              /* 10*a(i-3) */
        sum = sum - sub;                            /* 5*(a(i-1)+a(i-4)) - 10*a(i-3) */
        cache[i] = sum;
    }
    cache_size = new_size;

    return cache[n];
}
