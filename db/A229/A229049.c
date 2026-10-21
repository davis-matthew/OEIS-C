/*
 * A229049  G.f.: Sum_{n >= 0} (6*n)!/n!^6 * x^n / (1-x)^(6*n+1).
 * https://oeis.org/A229049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k) * C(n+k,k) * C(n+2k,k) * C(n+3k,k) *
 *            C(n+4k,k) * C(n+5k,k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of computed terms (next free index) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Allocate or grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 2 * cache_cap) new_cap = 2 * cache_cap;
        if (new_cap == 0) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t cur = cache_size; cur <= n; ++cur) {
        seq_t sum = 0;

        for (idx_t k = 0; k <= cur; ++k) {
            seq_t term = 1;
            term = term * binomial(cur, k);
            term = term * binomial(cur + k, k);
            term = term * binomial(cur + 2 * k, k);
            term = term * binomial(cur + 3 * k, k);
            term = term * binomial(cur + 4 * k, k);
            term = term * binomial(cur + 5 * k, k);
            sum = sum + term;
        }

        cache[cur] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
