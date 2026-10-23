/*
 * A168858  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A168858:
 * a(0)      = 1
 * a(n)      = rhs(n) + 35 * Σ₁^19 a(n-i) − 630 * a(n-20)   (n ≥ 1)
 * where
 *   rhs(n) = 1                       if n = 0 or n = 20
 *   rhs(n) = 2                       if 1 ≤ n ≤ 19
 *   rhs(n) = 0                       otherwise
 */

#include <stdlib.h>

/* External integer types are supplied by the build system */
seq_t a(idx_t n)
{
    /* Memoisation cache of already computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_n = 0;          /* number of elements currently stored */

    /* Ensure the cache can hold up to index n */
    if (n + 1 > cache_n) {
        idx_t new_n = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_n * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely */
        cache = tmp;

        /* Compute all new terms from old cache_n up to n */
        for (idx_t k = cache_n; k <= n; ++k) {
            seq_t rhs;
            if (k == 0 || k == 20)
                rhs = (seq_t)1;
            else if (k >= 1 && k <= 19)
                rhs = (seq_t)2;
            else
                rhs = (seq_t)0;

            /* Sum of the previous up to 19 terms */
            seq_t sum_prev = (seq_t)0;
            for (idx_t j = 1; j <= 19 && k >= j; ++j)
                sum_prev += cache[k - j];

            /* Take 630 · a(k-20) if it exists */
            seq_t prev20 = (seq_t)0;
            if (k >= 20)
                prev20 = cache[k - 20];

            /* Build the new term */
            seq_t temp = rhs + (seq_t)35 * sum_prev;
            if (k >= 20)
                temp -= (seq_t)630 * prev20;
            cache[k] = temp;
        }
        cache_n = new_n;
    }
    return cache[n];
}
