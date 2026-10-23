/*
 * A038732  T(n,n-3), array T as in A038730.
 * https://oeis.org/A038732
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   A038732 (T(n, n-3) from triangle A038730). Generating function:
 *            A(x) = x^3/[(1-3x+x^2)(1-x)^5]. Recurrence for the shifted
 *            sequence b_k = a(k+3): b_k = 8 b_{k-1} - 26 b_{k-2} + 45 b_{k-3}
 *            - 45 b_{k-4} + 26 b_{k-5} - 8 b_{k-6} + b_{k-7} (k ≥ 7)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n) {
    if (n < 3) return 0;            /* offset is 3 */
    idx_t need = n - 3;             /* index into b sequence */

    /* static cache: b_0, b_1, … */
    static seq_t *cache = NULL;     /* array of b values */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_len = 0;    /* number of initialized entries */

    /* initialize cache on first use */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;       /* allocation failed */
        /* initial values b_0 … b_6 from the OEIS list */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)38;
        cache[3] = (seq_t)141;
        cache[4] = (seq_t)455;
        cache[5] = (seq_t)1350;
        cache[6] = (seq_t)3805;
        cache_len = 7;
    }

    /* enlarge cache if needed */
    if (need >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= need) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend cache to 'need' using the recurrence */
    if (need >= cache_len) {
        for (idx_t i = (idx_t)cache_len; i <= need; ++i) {
            /* compute b_i = 8*b_{i-1} - 26*b_{i-2} + 45*b_{i-3}
               - 45*b_{i-4} + 26*b_{i-5} - 8*b_{i-6} + b_{i-7}   for i≥7
               For i < 7 the values are already initialized.          */
            seq_t pos = (seq_t)8 * cache[i-1]
                      + (seq_t)45 * cache[i-3]
                      + (seq_t)26 * cache[i-5]
                      + cache[i-7];
            seq_t neg = (seq_t)26 * cache[i-2]
                      + (seq_t)45 * cache[i-4]
                      + (seq_t)8  * cache[i-6];
            cache[i] = pos - neg;
        }
        cache_len = need + 1;
    }

    return cache[need];
}
