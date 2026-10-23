/*
 * A049133  Revert transform of (x - 1)^2/(1 - x - x^3).
 * https://oeis.org/A049133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=4. For n≥1 the sequence satisfies
 *            the binomial sum a(n+1) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} (-1)^k *
 *            binomial(n+1 , k) * binomial(2n-2k , n-3k). The function caches
 *            previously computed values so that calls in any order are fast
 *            and each term is evaluated exactly
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[1] … cache[cache_size] */
    static idx_t cache_size = 0;     /* largest index currently stored */

    if (n == 0) return 0;            /* undefined, but harmless */

    /* Grow cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n + 1;      /* indices 0..n, index 0 unused */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) exit(1);  /* minimal error handling */

        /* Initialize base cases if not already set */
        if (new_size > 1) cache[1] = (seq_t)1;
        if (new_size > 2) cache[2] = (seq_t)1;
        if (new_size > 3) cache[3] = (seq_t)2;
        if (new_size > 4) cache[4] = (seq_t)4;

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 4) continue;   /* base values already set */

            idx_t k_limit = (i - 1) / 3;   /* floor((i-1)/3) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= k_limit; ++k) {
                seq_t term1 = binomial(i, k);                          /* binomial(i, k) */
                seq_t term2 = binomial((idx_t)(2 * (i - 1) - 2 * k),
                                       (idx_t)(i - 1 - 3 * k));          /* binomial(2n-2k, n-3k) */
                seq_t term = term1 * term2;
                if (k & 1)        /* odd k: subtract */
                    sum -= term;
                else              /* even k: add */
                    sum += term;
            }
            seq_t divisor = (seq_t)i;    /* i = n+1 in the formula */
            cache[i] = sum / divisor;    /* exact division */
        }
        cache_size = n;
    }

    return cache[n];
}
