/*
 * A168718  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^17 = I.
 * https://oeis.org/A168718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; For n >= 1 let c(n) = 1 if n = 0 or n = 17, 2 if 1 <=
 *            n <= 16, 0 otherwise. Then the recurrence from the generating
 *            function a(n) - 39*(a(n-1)+...+a(n-16)) + 780*a(n-17) = c(n)
 *            holds for every n >= 0. Negative indices are taken as 0. This
 *            recurrence is used to compute a(n) for any n ≥ 0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t   cached_n = (idx_t)-1;   /* maximum n computed so far */

    /* initialise cache with a(0) */
    if (cached_n == (idx_t)-1) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) {
            /* If allocation fails, abort the program */
            abort();
        }
        cache[0] = (seq_t)1;       /* a(0) = 1 */
        cached_n = 0;
    }

    /* if the requested n is beyond the cached values, extend the cache */
    if (n > cached_n) {
        idx_t new_size = n + 1;                      /* capacity needed */
        seq_t *new_cache = (seq_t*)realloc(cache,
                                           new_size * sizeof(seq_t));
        if (!new_cache) {
            /* If reallocation fails, abort */
            abort();
        }
        cache = new_cache;

        /* compute a(k) for k = cached_n+1 .. n */
        for (idx_t k = cached_n + 1; k <= n; ++k) {
            /* sum of the previous 16 terms */
            seq_t sum = 0;
            for (idx_t t = 1; t <= 16; ++t) {
                if (k >= t)
                    sum += cache[k - t];
            }

            /* coefficient c(k) */
            seq_t c;
            if (k == 0 || k == 17)
                c = (seq_t)1;
            else if (1 <= k && k <= 16)
                c = (seq_t)2;
            else
                c = (seq_t)0;   /* for k > 17 */

            /* term from a(k-17) if it exists */
            seq_t term17 = 0;
            if (k >= 17)
                term17 = cache[k - 17];

            /* recurrence: a(k) = c(k) + 39*sum - 780*term17 */
            cache[k] = c + (seq_t)39 * sum - (seq_t)780 * term17;
        }

        cached_n = n;   /* update maximum computed index */
    }

    return cache[n];
}
