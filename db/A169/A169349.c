/*
 * A169349  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 46 * (a(n-1) + … + a(n-29)) – 1081 * a(n-30),
 *            where p(0) = p(30) = 1, p(1)…p(29) = 2, and p(k) = 0 for k > 30
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache  = NULL;  /* array of computed terms */
    static idx_t cache_size = 0; /* number of elements currently stored */

    if (n < cache_size)               /* already computed */
        return cache[n];

    /* Extend cache up to n. */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1); /* allocation failed */

    /* Compute missing terms. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        /* sum of the previous up to 29 terms */
        for (idx_t k = 1; k <= 29 && i >= k; ++k) {
            sum += tmp[i - k];
        }

        /* polynomial part p(n) */
        seq_t p = 0;
        if (i == 0 || i == 30)           p = 1;
        else if (i <= 29)                p = 2;

        seq_t term = p + 46 * sum;
        if (i >= 30)                     /* subtract 1081 * a(n-30) */
            term = term - 1081 * tmp[i - 30];

        tmp[i] = term;
    }

    cache = tmp;
    cache_size = new_size;
    return cache[n];
}
