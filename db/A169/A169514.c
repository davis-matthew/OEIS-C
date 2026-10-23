/*
 * A169514  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169514: Number of reduced words of length n
 * in a Coxeter group with 21 generators and relations S_i^2 = (S_i S_j)^34 = I.
 *
 * Recurrence derived from the rational generating function:
 *
 *   a(0) = 1
 *   For 1 ≤ n ≤ 33:
 *       a(n) = 19 * Σ_{k=0}^{n-1} a(k) + 2
 *   For n ≥ 34:
 *       a(n) = 19 * Σ_{k=n-33}^{n-1} a(k) - 190 * a(n-34) + [n == 34]
 *
 * The bracket term is 1 when n = 34, otherwise 0.
 *
 * The values are generated on demand and cached so that calls may
 * arrive in any order.  Complexity is O(n) per new element, with
 * constant factor 34 for recurrence computations.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;       /* number of terms stored */

    /* Extend the cache if n is beyond the current size */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_len = new_len;

        /* Compute new terms */
        for (idx_t i = 0; i < cache_len; ++i) {
            seq_t val;

            if (i == 0) {
                /* a(0) = 1 */
                val = (seq_t)1;
            } else if (i <= 33) {
                /* a(i) = 19 * sum_{k=0}^{i-1} a(k) + 2 */
                seq_t sumPrev = (seq_t)0;
                for (idx_t k = 0; k < i; ++k) {
                    sumPrev += cache[(size_t)k];
                }
                val = (seq_t)19 * sumPrev + (seq_t)2;
            } else {
                /* a(i) = 19 * sum_{k=i-33}^{i-1} a(k) - 190 * a(i-34) + N(i) */
                seq_t sumLast33 = (seq_t)0;
                for (idx_t j = 1; j <= 33; ++j) {
                    sumLast33 += cache[(size_t)(i - j)];
                }
                val = (seq_t)19 * sumLast33;
                /* Add the constant term N(i) */
                if (i == 34) {
                    val += (seq_t)1;
                }
                /* Subtract 190 * a(i-34) */
                val -= (seq_t)190 * cache[(size_t)(i - 34)];
            }

            cache[(size_t)i] = val;
        }
    }

    return cache[(size_t)n];
}
