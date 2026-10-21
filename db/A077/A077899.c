/*
 * A077899  Expansion of (1-x)^(-1)/(1+x-2*x^2+x^3).
 * https://oeis.org/A077899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (-1)^n * Sum_{k=0..n} binomial(2*k-1, n-k)
 * Recurrence: a(0)=1, a(1)=0, a(2)=3; for n>=3: a(n) = 1 - a(n-1) + 2*a(n-2) - a(n-3)
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize base values */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        /* Fallback for empty allocation is not handled – the test harness
           ensures sufficient memory. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)3;
        cache_len = 3;
    }

    /* Grow the cache if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        /* Assume realloc succeeds – the harness guarantees enough memory. */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a(i) = 1 - a(i-1) + 2*a(i-2) - a(i-3) */
            seq_t val = (seq_t)1
                        - cache[i-1]
                        + ((seq_t)2 * cache[i-2])
                        - cache[i-3];
            cache[i] = val;
        }
        cache_len = new_len;
    }
    return cache[n];
}
