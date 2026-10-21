/*
 * A081020  Even order Taylor coefficients at x = 0 of exp(-x^2/(x^2-2)), odd
 *          order coefficients being equal to zero.
 * https://oeis.org/A081020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 1 For n >= 2: a(n) = (2*(n-1)+1)^2 * a(n-1) -
 *            (n-1)*(n-2)*(2*(n-1)-1)*(2*(n-1)+1) * a(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* allocated size (number of entries)  */
    static idx_t  max_n = (idx_t)-1; /* largest index that has been computed */

    /* initial allocation: at least two entries for a(0) and a(1) */
    if (cache == NULL) {
        cap = (n >= 1) ? n + 1 : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        if (cap > 1) {
            cache[1] = (seq_t)1;
        }
        max_n = (cap > 1) ? 1 : 0;
    }

    /* grow cache if needed */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – abort as per contest policies */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    if (n > max_n) {
        for (idx_t idx = max_n + 1; idx <= n; ++idx) {
            if (idx <= 1) {
                cache[idx] = (seq_t)1;
            } else {
                idx_t k = idx - 1; /* recurrence uses k = idx-1 */
                seq_t term1 = ((seq_t)(2 * k + 1) * (seq_t)(2 * k + 1)) * cache[k];
                seq_t term2 = ((seq_t)k * (seq_t)(k - 1)) *
                              (seq_t)(2 * k - 1) *
                              (seq_t)(2 * k + 1) *
                              cache[k - 1];
                cache[idx] = term1 - term2;
            }
        }
        max_n = n;
    }

    return cache[n];
}
