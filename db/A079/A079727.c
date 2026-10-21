/*
 * A079727  a(n) = 1 + C(2,1)^3 + C(4,2)^3 + ... + C(2n,n)^3.
 * https://oeis.org/A079727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k)^3 (OEIS A079727)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* static cache for memoization (sequence values already computed) */
    static seq_t *cache      = NULL;    /* pointer to cached values */
    static idx_t cache_len   = 0;       /* number of values currently stored */
    static size_t cache_cap  = 0;       /* allocated capacity of the cache */

    /* grow cache if necessary to hold values up to index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp;
        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize cache if it is empty */
    if (cache_len == 0) {
        cache[0] = 1;       /* a(0) = 1 */
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t c   = binomial(k + k, k);   /* central binomial coefficient */
        seq_t val = c * c * c;            /* c³ */
        cache[k] = cache[k - 1] + val;
    }

    cache_len = n + 1;
    return cache[n];
}
