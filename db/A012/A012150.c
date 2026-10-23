/*
 * A012150  Expansion of e.g.f. exp(tan(arcsin(x))).
 * https://oeis.org/A012150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n^2 - 12*n + 13)*a(n-2) - 3*(n-4)*(n-3)^2*(n-2)*a(n-4)
 *            + (n-6)*(n-5)*(n-4)^2*(n-3)*(n-2)*a(n-6) Initial terms: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=4, a(4)=13, a(5)=76
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;   /* highest index computed so far */

    /* initialise cache with first 6 known terms */
    if (cache == NULL) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failure (unlikely) */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)76;
        cache_max = 5;
    }

    /* enlarge cache when needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap < (size_t)n + 1) {
            new_cap = (size_t)n + 1;
            while (new_cap < (size_t)n + 1)
                new_cap <<= 1;                /* exponential growth */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return already computed value */
    if (n <= cache_max)
        return cache[n];

    /* compute terms from cache_max+1 up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* coefficients computed in seq_t to avoid overflow */
        seq_t coeff1 = (((seq_t)3 * (seq_t)i * (seq_t)i)
                        - ((seq_t)12 * (seq_t)i) + (seq_t)13);
        seq_t term1 = coeff1 * cache[i - 2];

        seq_t coeff2 = (seq_t)3;
        coeff2 *= (seq_t)(i - 4);
        coeff2 *= (seq_t)(i - 3);
        coeff2 *= (seq_t)(i - 3);
        coeff2 *= (seq_t)(i - 2);
        seq_t term2 = coeff2 * cache[i - 4];

        seq_t coeff3 = (seq_t)(i - 6);
        coeff3 *= (seq_t)(i - 5);
        coeff3 *= (seq_t)(i - 4);
        coeff3 *= (seq_t)(i - 4);
        coeff3 *= (seq_t)(i - 3);
        coeff3 *= (seq_t)(i - 2);
        seq_t term3 = coeff3 * cache[i - 6];

        cache[i] = term1 - term2 + term3;
    }

    cache_max = n;
    return cache[n];
}
