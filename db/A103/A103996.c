/*
 * A103996  Recurrence: a(n) = -Sum[i=0..n-1, a(i)*C(n+1,i) ], a(0)=1.
 * https://oeis.org/A103996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{i=0}^{n-1} a(i) * C(n+1,i), a(0)=1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 14, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t size = 0;   /* number of terms currently stored (size = last_index+1) */

    /* ensure cache initialized and contains term 0 */
    if (size == 0) {
        size_t newcap = 1;
        cache = (seq_t *)realloc(NULL, newcap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return zero (unlikely) */
        cache[0] = (seq_t)1; /* a(0) = 1 */
        cap = newcap;
        size = 1;
    }

    /* if requested n already computed, just return it */
    if (n < size) return cache[n];

    /* grow cache to accommodate indices up to n */
    if (n >= (idx_t)cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms from current size up to n */
    for (idx_t m = size; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1; /* C(m+1,0) */
        for (idx_t i = 0; i < m; ++i) {
            sum += cache[i] * binom;
            /* compute next binomial coefficient:
               C(m+1, i+1) = C(m+1, i) * (m+1 - i) / (i+1) */
            if (i + 1 < m) {
                seq_t mult = binom * (seq_t)(m + 1 - i);
                binom = mult / (seq_t)(i + 1);
            }
        }
        cache[m] = -sum;
    }
    size = n + 1;
    return cache[n];
}
