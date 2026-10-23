/*
 * A167148  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥0 a(n) = 18·Σ_{k=1}^{13} a(n−k) − 171·a(n−14) +
 *            p(n), where p(n)=1 if n==0 or n==14, p(n)=2 if 1≤n≤13, otherwise
 *            p(n)=0. The values are generated exactly from this recurrence
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* expand cache to hold up to n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        if (cache == NULL) {
            cache      = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache      = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }

        /* compute missing terms */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t p = 0;
            if (i == 0 || i == 14) {
                p = 1;
            } else if (i >= 1 && i <= 13) {
                p = 2;
            }                   /* else p stays 0 */

            seq_t sum = 0;
            for (idx_t k = 1; k <= 13; ++k) {
                if (i >= k) {
                    sum += cache[(size_t)(i - k)];
                }
            }

            seq_t term = p + (seq_t)18 * sum;
            if (i >= 14) {
                term -= (seq_t)171 * cache[(size_t)(i - 14)];
            }
            cache[(size_t)i] = term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
