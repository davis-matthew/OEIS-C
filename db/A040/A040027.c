/*
 * A040027  The Gould numbers.
 * https://oeis.org/A040027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0)=1, a(1)=1, for n>1: a(n) = n + sum_{j=1}^{n-1} binomial(n ,
 *            j+1) * a(j)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed values */
    static seq_t *cache = NULL;          /* array of a[0..cached_max] */
    static idx_t cached_max = (idx_t)-1; /* highest n that has been computed */
    static size_t cache_cap = 0;         /* allocated capacity (in elements) */

    /* allocate initial cache if required */
    if (cache == NULL) {
        /* start small, will grow when needed */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;       /* allocation failed */
        cache[0] = (seq_t)1;                      /* a(0) */
        cache[1] = (seq_t)1;                      /* a(1) */
        cached_max = 1;
    }

    /* grow cache capacity if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) new_cap <<= 1; /* double until large enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;         /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    while (cached_max < n) {
        idx_t i = cached_max + 1;
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1; /* binomial(i,0) */

        /* compute binomial(i, k) successively for k = 1..i */
        for (idx_t k = 1; k <= i; ++k) {
            /* binom = binom * (i - k + 1) / k */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            if (k >= 2) { /* we need C(i, j+1) where j = k-1 */
                idx_t j = k - 1;
                if (j >= 1 && j <= i - 1) {
                    sum += binom * cache[j];
                }
            }
        }
        cache[i] = (seq_t)i + sum;
        ++cached_max;
    }

    return cache[n];
}
