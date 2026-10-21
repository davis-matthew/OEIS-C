/*
 * A063282  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          9 ).
 * https://oeis.org/A063282
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 with initial values:
 *            a(2)= -1, a(3)=0, a(4)=2, a(5)=5, a(6)=8, a(7)=9, a(8)=12,
 *            a(9)=15, a(10)=16, a(11)=19, a(12)=22, a(13)=24 (values for n <
 *            2 are irrelevant)
 * Range:     i8 n <= 56, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* store computed terms; indices 0 and 1 unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 1;          /* maximum n for which cache[n] is valid */

    /* initial values a(2) … a(13) */
    static const seq_t init_vals[14] = {
        0,      /* 0 */
        0,      /* 1 */
        -1,     /* 2 */
         0,     /* 3 */
         2,     /* 4 */
         5,     /* 5 */
         8,     /* 6 */
         9,     /* 7 */
        12,     /* 8 */
        15,     /* 9 */
        16,     /*10 */
        19,     /*11 */
        22,     /*12 */
        24      /*13 */
    };

    if (n < 2)                     /* may be called with smaller n; return 0 */
        return (seq_t)0;

    /* enlarge cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        /* ensure a bit more room for future growth */
        if (new_size < 256) new_size = 256;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* initialize new entries to zero (they will be overwritten soon) */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* if already computed up to n, just return */
    if (n <= max_computed)
        return cache[n];

    /* compute from max_computed+1 up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = init_vals[i];
        } else {
            /* recurrence a(n) = a(n-4) + a(n-6) - a(n-10) */
            cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
        }
    }
    max_computed = n;
    return cache[n];
}
