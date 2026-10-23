/*
 * A168802  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^19 = I.
 * https://oeis.org/A168802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) – 27·∑_{j=1}^{18}
 *            a(n−j) + 378·a(n−19) = p(n), * where p(n) = 1 for n = 0 or 19,
 *            p(n) = 2 for 1 ≤ n ≤ 18, * and p(n) = 0 otherwise. This comes
 *            from the rational * generating function supplied in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static size_t      cap   = 0;   /* allocated size */
    static idx_t       len   = 0;   /* number of terms stored */

    /* Return immediately if the term is already cached. */
    if (n < len)
        return cache[n];

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Grow cache if necessary. */
    if (cap <= (size_t)n) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Compute terms from the current length up to n. */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        /* Sum of the last 18 terms: a(i-1) + … + a(i-18). */
        for (idx_t j = 1; j <= 18 && i >= j; ++j)
            sum += cache[i - j];

        seq_t prev19 = (i >= 19) ? cache[i - 19] : (seq_t)0;
        seq_t p;
        /* The inhomogeneous part p(i). */
        if (i == 0)
            p = (seq_t)1;
        else if (i <= 18)
            p = (seq_t)2;
        else if (i == 19)
            p = (seq_t)1;
        else
            p = (seq_t)0;

        /* Recurrence: a(i) = p + 27·sum – 378·prev19. */
        cache[i] = p + (seq_t)27 * sum - (seq_t)378 * prev19;
    }

    len = n + 1;
    return cache[n];
}
