/*
 * A041860  Numerators of continued fraction convergents to sqrt(452).
 * https://oeis.org/A041860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent of sqrt(452)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Periodic coefficients of the continued fraction for sqrt(452)
       after the initial term a0 = 21.  The period length is 10. */
    static const seq_t period[10] = {
        (seq_t)3, (seq_t)1, (seq_t)5, (seq_t)3, (seq_t)10,
        (seq_t)3, (seq_t)5, (seq_t)1, (seq_t)3, (seq_t)42
    };

    /* Cache of already computed numerators.  cache[0] = p0 = 21. */
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cache_cap = 0;      /* allocated capacity */
    static seq_t *cache = NULL;      /* array of numerators */

    if (cache_len == 0) {
        cache_cap = 16;                     /* initial capacity */
        cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        cache[0] = (seq_t)21;                /* p0 */
        cache_len = 1;
    }

    /* Return a cached value if available. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache capacity if needed. */
    if (n >= cache_cap) {
        while (cache_cap <= (size_t)n)
            cache_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)calloc((size_t)cache_cap, sizeof(seq_t));
        memcpy(new_cache, cache, (size_t)cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }

    /* Compute terms up to the requested index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t a_coeff = period[(i - 1) % 10];   /* a_i for i >= 1 */

        seq_t val;
        if (i == 1) {
            /* p1 = a1 * p0 + p_{-1},  where p_{-1} = 1 */
            val = a_coeff * cache[0] + (seq_t)1;
        } else {
            /* p_i = a_i * p_{i-1} + p_{i-2} for i >= 2 */
            val = a_coeff * cache[i - 1] + cache[i - 2];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
