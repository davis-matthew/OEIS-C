/*
 * A201355  Expansion of e.g.f.: 3*exp(3*x) / (4 - exp(3*x)).
 * https://oeis.org/A201355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n+1) = 3*a(n) + Sum_{k=0}^{n} binomial(n, k) * a(k) *
 *            a(n-k) for n ≥ 0 This recurrence follows from the e.g.f. E(x) =
 *            3*exp(3x)/(4 - exp(3x))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size   = 0;   /* number of values stored (cache_size = current max index + 1) */
    static idx_t cache_cap    = 0;   /* allocated capacity of cache */

    /* Initialise cache with a(0) = 1 */
    if (cache == NULL) {
        cache_cap  = 1;
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
    }

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow cache if needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;      /* double until sufficient */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Compute new terms up to index n */
    for (idx_t m = cache_size; m <= n; ++m) {
        idx_t kmax = m - 1;
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;        /* binomial(m-1, 0) = 1 */

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t term = binom * cache[k] * cache[kmax - k];
            sum += term;

            if (k < kmax) {
                binom = binom * (idx_t)(kmax - k);   /* multiply by (m-1-k) */
                binom = binom / (idx_t)(k + 1);     /* divide by (k+1) */
            }
        }

        /* a(m) = sum + 3 * a(m-1) */
        cache[m] = sum + cache[m - 1] * (seq_t)3;
    }

    cache_size = n + 1;   /* now cache[0..n] are valid */
    return cache[n];
}
