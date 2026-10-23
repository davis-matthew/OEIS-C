/*
 * A170127  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1..37)=2, a(38)=1, and for n≥0 a(n) =
 *            20*∑_{k=1}^{min(37,n)} a(n-k) – (n≥38?210*a(n-38):0) + c(n)
 *            where c(n)=1 if n=0 or 38, c(n)=2 if 1≤n≤37, else c(n)=0. This
 *            recurrence follows from the given rational generating function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache for already computed terms. */
    static seq_t *cache = NULL;          /* array of computed a(i) */
    static idx_t  cache_size = 0;        /* allocated capacity */
    static idx_t  last_computed = (idx_t)-1;   /* last index for which a(i) was computed */

    /* Ensure the cache is large enough for indices up to n. */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        /* copy previously computed values */
        if (cache) {
            for (idx_t i = 0; i <= last_computed; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_size = n + 1;
    }

    /* Determine starting point for incremental computation. */
    idx_t start = (last_computed == (idx_t)-1) ? 0 : last_computed + 1;

    /* Compute terms that have not yet been computed. */
    for (idx_t i = start; i <= n; ++i) {
        seq_t sumPrev = 0;
        for (idx_t k = 1; k <= 37 && i >= k; ++k)
            sumPrev += cache[i - k];

        seq_t val = sumPrev * (seq_t)20;
        if (i >= 38)
            val -= cache[i - 38] * (seq_t)210;

        if (i == 0)
            val += (seq_t)1;
        else if (i <= 37)
            val += (seq_t)2;
        else if (i == 38)
            val += (seq_t)1;

        cache[i] = val;
    }

    last_computed = (last_computed == (idx_t)-1) ? n : (last_computed > n ? last_computed : n);

    return cache[n];
}
