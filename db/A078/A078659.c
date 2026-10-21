/*
 * A078659  Expansion of 1/Product_{ n >= 2, n not of the form 2^k-1 }
 *          (1+x^n).
 * https://oeis.org/A078659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in the expansion of 1 / ∏_{m≥2,
 *            m≠2^k−1} (1 + x^m)
 * Range:     i8 n <= 177, i16 n <= 409, i32 n <= 961, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of the computed coefficients */
    static seq_t *cached = NULL;
    static idx_t cached_limit = 0;   /* last index that has been computed */

    /* Recompute the whole table when needed */
    if (!cached || n > cached_limit) {
        idx_t new_limit = n;

        /* Allocate a new array big enough for all indices up to n */
        seq_t *new_cache = (seq_t*)malloc((new_limit + 1) * sizeof(seq_t));
        if (!new_cache) return 0;     /* allocation failure – return 0 */

        /* initialise coefficients: a(0)=1, others 0 */
        for (idx_t i = 0; i <= new_limit; ++i) new_cache[i] = 0;
        new_cache[0] = (seq_t)1;

        /* Process all allowed parts m = 2, 3, …, n                */
        /* A part m is skipped if m+1 is a power of two          */
        for (idx_t m = 2; m <= new_limit; ++m) {
            if (((m + 1) & m) == 0) continue;            /* m is Mersenne, skip */
            for (idx_t k = m; k <= new_limit; ++k) {
                new_cache[k] -= new_cache[k - m];
            }
        }

        /* Replace the old cache by the new one */
        free(cached);
        cached       = new_cache;
        cached_limit = new_limit;
    }

    return cached[n];
}
