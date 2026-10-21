/*
 * A061684  Generalized Bell numbers: column 3 of A275043.
 * https://oeis.org/A061684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * sum_{k=0..n-1} binomial(n,k)^3 * (n-k) * a(k) ;
 *            a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* expand cache to hold terms up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* memory allocation failed */
        cache = tmp;
        /* initialize new slots to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }

    /* initialise a(0) */
    if (cache[0] == 0)
        cache[0] = 1;

    if (n == 0)
        return cache[0];

    /* compute all terms up to n (inclusive) */
    for (size_t m = 1; m <= (size_t)n; ++m) {
        if (cache[m] != 0)                  /* already computed */
            continue;

        seq_t sum = 0;
        seq_t bin = 1;                      /* binomial(m,0) */

        for (size_t k = 0; k < m; ++k) {
            /* term = a(k) * (m-k) * bin^3 */
            seq_t term = cache[k];
            term *= (seq_t)(m - k);
            seq_t bin3 = bin * bin * bin;
            term *= bin3;
            sum += term;

            /* update binomial to binomial(m, k+1) */
            if (k + 1 < m) {
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                bin *= num;
                bin /= den;
            }
        }
        /* divide by m */
        cache[m] = sum / (seq_t)m;
    }

    return cache[n];
}
