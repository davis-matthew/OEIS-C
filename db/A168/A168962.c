/*
 * A168962  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence coming from the generating function of A168962 a(0) =
 *            1 for 1 ≤ n ≤ 21 : a(n) = 43 * (a(n-1)+...+a(n-min(n,21))) + 2
 *            for n ≥ 22 : a(n) = 43 * (a(n-1)+...+a(n-21)) – 946 * a(n-22)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* overall length allocated */

    /* allocate enough space for indices up to n */
    if ((size_t)(n + 1) > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;        /* allocation failed */

        /* copy already computed values */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* initialize the newly allocated part to zero */
        for (size_t i = cache_len; i < new_len; ++i)
            new_cache[i] = (seq_t)0;

        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* ensure a(0) is set */
    if (cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* compute any missing terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        /* sum of the previous at most 21 terms */
        seq_t sum = (seq_t)0;
        size_t up = (i <= 21) ? (size_t)i : (size_t)21;
        for (size_t k = 1; k <= up; ++k)
            sum += cache[i - k];

        seq_t term;
        if (i <= 21) {
            /* a(n) = 43*sum + 2 */
            term = (seq_t)43 * sum + (seq_t)2;
        } else {
            /* a(n) = 43*sum – 946*a(n-22) */
            term = (seq_t)43 * sum - (seq_t)946 * cache[i - 22];
        }
        cache[i] = term;
    }

    return cache[n];
}
