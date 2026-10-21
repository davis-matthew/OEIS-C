/*
 * A052684  Expansion of e.g.f. 1/(1-2*x^2-x^3).
 * https://oeis.org/A052684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(n-1)*a(n-2) + n*(n-1)*(n-2)*a(n-3), a(0)=1, a(1)=0,
 *            a(2)=4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated length (in elements) */
    static idx_t max_comp   = 0;   /* greatest n that has been computed */

    /* initial allocation for the first three terms */
    if (cache == NULL) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        max_comp = 2;
    }

    /* expand the cache if we need to compute higher terms */
    if (n > max_comp) {
        idx_t required_len = n + 1;
        if (required_len > cache_len) {
            idx_t new_len = cache_len;
            while (new_len < required_len) new_len <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* reallocation failed */
            cache = tmp;
            cache_len = new_len;
        }

        for (idx_t i = max_comp + 1; i <= n; ++i) {
            /* for i >= 3 the recurrence applies */
            seq_t term1 = (seq_t)2 * (seq_t)i * (seq_t)(i - 1) * cache[i - 2];
            seq_t term2 = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2) * cache[i - 3];
            cache[i] = term1 + term2;
        }
        max_comp = n;
    }

    return cache[n];
}
