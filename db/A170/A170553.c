/*
 * A170553  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170553
 * recurrence derived from the generating function
 *
 * a(0) = 1
 * for 1 ≤ n ≤ 46: a(n) = 2 + 14*(a(n-1)+a(n-2)+…+a(0))
 * a(47)    = 1 + 14*(a(46)+a(45)+…+a(1))
 * for n ≥ 48:   a(n) = 14*(a(n-1)+a(n-2)+…+a(n-46)) - 105*a(n-47)
 *
 * The coefficients are computed iteratively and cached for efficiency.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* allocated length */
    static idx_t  last_computed = (idx_t)-1;  /* index of last computed value */

    /* enlarge cache if necessary */
    if (cache_size <= (size_t)n) {
        size_t new_size = ((size_t)n + 1) * 2;
        if (cache == NULL) {
            cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        cache_size = new_size;
    }

    /* compute missing values */
    if (last_computed == (idx_t)-1) {          /* first call */
        last_computed = 0;
        if (n >= 0)
            cache[0] = (seq_t)1;
    }

    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            /* compute linear combination of the last 46 terms (or fewer) */
            seq_t sum14 = 0;
            idx_t m = (i <= 46 ? i : 46);                 /* at most 46 terms */
            for (idx_t k = 1; k <= m; ++k) {
                sum14 += (seq_t)14 * cache[i - k];
            }

            if (i == 0) {
                val = (seq_t)1;
            } else if (i <= 46) {
                /* N_i = 2 for 1 ≤ i ≤ 46 */
                val = sum14 + (seq_t)2;
            } else if (i == 47) {
                /* N_47 = 1 */
                val = sum14 + (seq_t)1;
            } else { /* i ≥ 48 */
                /* subtract the 105*a[i-47] term */
                val = sum14 - (seq_t)105 * cache[i - 47];
            }
            cache[i] = val;
        }
        last_computed = n;
    }
    return cache[n];
}
