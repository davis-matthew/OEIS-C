/*
 * A063330  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          57 ).
 * https://oeis.org/A063330
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n‑1) – a(n‑3) + 2·a(n‑4) – a(n‑5) + a(n‑7) – a(n‑8) for
 *            n ≥ 12 Initial values: a(0)=0, a(1)=0, a(2)=–1, a(3)=71,
 *            a(4)=162, a(5)=252, a(6)=342, a(7)=430, a(8)=520, a(9)=610,
 *            a(10)=698, a(11)=788 (These are derived from the generating
 *            function)
 * Range:     i8 n <= 3, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initial terms up to n=11 */
    static const seq_t init[12] = {
        0,    /* a(0) */
        0,    /* a(1) */
        -1,   /* a(2) */
        71,   /* a(3) */
        162,  /* a(4) */
        252,  /* a(5) */
        342,  /* a(6) */
        430,  /* a(7) */
        520,  /* a(8) */
        610,  /* a(9) */
        698,  /* a(10) */
        788   /* a(11) */
    };

    /* extend cache if needed */
    if (n >= cache_len) {
        size_t wanted = n + 1;
        if (cache_cap < wanted) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            while (new_cap < wanted) new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) exit(1);  /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* fill missing values */
        for (idx_t i = cache_len; i < wanted; ++i) {
            seq_t val;
            if (i < 12) {
                val = init[i];
            } else {
                /* recurrence:
                   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8)  */
                val = cache[i-1]
                    - cache[i-3]
                    + 2 * cache[i-4]
                    - cache[i-5]
                    + cache[i-7]
                    - cache[i-8];
            }
            cache[i] = val;
        }
        cache_len = wanted;
    }

    /* return term */
    return cache[n];
}
