/*
 * A165687  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the generating function G(t) = (t^9 + 2*t^8 +
 *            2*t^7 + 2*t^6 + 2*t^5 + 2*t^4 + 2*t^3 + 2*t^2 + 2*t + 1) /
 *            (666*t^9 - 36*t^8 - 36*t^7 - 36*t^6 - 36*t^5 - 36*t^4 - 36*t^3 -
 *            36*t^2 - 36*t + 1) which yields the linear recurrence a(n) =
 *            p(n) + 36*∑_{i=1}^{8} a(n-i) - 666*a(n-9) where...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;          /* allocated capacity */
    static size_t cache_len = 0;          /* number of values stored (cache_len-1 is last valid index) */

    size_t req = (size_t)n + 1;  /* number of values needed */

    /* Allocate or enlarge cache as necessary */
    if (cache_cap < req) {
        size_t new_cap = req;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort (undefined behaviour) */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to index n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* Compute sum of previous eight terms */
        seq_t sum = 0;
        for (idx_t k = 1; k <= 8 && i >= k; ++k) {
            sum += cache[i - k];
        }

        /* Term from recurrence */
        seq_t val = 36 * sum;
        if (i >= 9) {
            val -= 666 * cache[i - 9];
        }

        /* Add numerator coefficient p(i) */
        seq_t p;
        if (i == 0) {
            p = 1;
        } else if (i <= 8) {
            p = 2;
        } else if (i == 9) {
            p = 1;
        } else {
            p = 0;
        }

        cache[i] = val + p;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
