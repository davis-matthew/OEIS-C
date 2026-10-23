/*
 * A170048  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 37*∑_{k=1}^{35}a(n−k) − 703*a(n−36) + b(n) * where
 *            b(0)=1, b(1)…b(35)=2, b(36)=1, b(n)=0 for n>36. * This
 *            recurrence follows from the generating function * G(t) = (t^36 +
 *            2t^35 + … + 2t + 1) / (703t^36 − 37t^35 − … − 37t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;     /* number of entries stored */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;                /* a(0) = 1 */
    }

    /* If the requested n is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Resize cache to at least n+1 entries */
    size_t new_len = (size_t)(n + 1);
    if (new_len > (size_t)cache_len) {
        size_t old_len = (size_t)cache_len;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;         /* allocation failed */
        /* Copy existing values */
        for (size_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_len = (idx_t)new_len;

        /* Compute missing terms from old_len to n (inclusive) */
        for (idx_t i = (idx_t)old_len; i <= n; ++i) {
            seq_t term = (seq_t)0;

            /* 37 * (a[i−1] + … + a[i−35])  */
            for (idx_t k = 1; k <= 35 && k <= i; ++k)
                term += cache[i - k] * (seq_t)37;

            /* −703 * a[i−36] for i ≥ 36 */
            if (i >= 36)
                term -= cache[i - 36] * (seq_t)703;

            /* Add the inhomogeneous part from the numerator */
            if (i == 0 || i == 36)
                term += (seq_t)1;
            else if (i <= 35)
                term += (seq_t)2;
            /* else term += 0 */

            cache[i] = term;
        }
    } else {
        /* Cache was only enlarged to new_len, but no new terms need
         * computation because n < new_len was already handled above. */
    }

    return cache[n];
}
