/*
 * A165708  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165708
   Rational generating function:
      G(t) = (1 + 2t + 2t² + ··· + 2t⁸ + t⁹) /
             (1 - 46t - 46t² - 46t³ - 46t⁴ - 46t⁵ - 46t⁶ - 46t⁷ - 46t⁸ + 1081t⁹)
   The coefficients satisfy the linear recurrence
      a(n) = 46·a(n‑1) + … + 46·a(n‑8) – 1081·a(n‑9) + b(n)
   where
      b(0) =  1,   b(1…8) = 2,   b(9) = 1,   b(n≥10) = 0
   The offset is 0 (a(0) = 1).  The routine caches values between
   calls so that each term is computed at most once. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* coefficient of the numerator of G(t) */
    static const int n_coef[10] = {
        1, 2, 2, 2, 2, 2, 2, 2, 2, 1
    };

    /* cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* how many terms are stored (next free index) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* helper: reallocate cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first term if cache empty */
    if (cache_len == 0)
        cache[0] = (seq_t)1, cache_len = 1;

    /* compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = 0;

        /* add contribution from the numerator coefficient */
        if (i <= 9)
            term += (seq_t)n_coef[i];

        /* sum of previous eight terms with coefficient 46 */
        for (idx_t k = 1; k <= 8 && k <= i; ++k)
            term += (seq_t)46 * cache[i - k];

        /* subtract 1081·a(i‑9) if it exists */
        if (i >= 9)
            term -= (seq_t)1081 * cache[i - 9];

        cache[i] = term;
    }
    cache_len = n + 1;               /* update stored range */

    return cache[n];
}
