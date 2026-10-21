/*
 * A064347  Generalized Catalan numbers C(9,9; n).
 * https://oeis.org/A064347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((9^(2*(n-1)))/(n-1))*sum_{m=0}^{n-2}
 *            (m+1)*(m+2)*binomial(2*(n-2)-m, n-2-m)*(1/9)^(m+1), recurrence:
 *            a(0)=a(1)=1, for n>=2: 8*(n-1)*a(n) + 9*(287*n-720)*a(n-1) +
 *            1458*(2*n-3)*a(n-2) = 0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL; /* cached values, cache[i] == a(i) */
    static size_t len   = 0;    /* number of initialized entries (1st valid is index 0) */
    static size_t cap   = 0;    /* current allocation size */

    /* extend cache if necessary – allows non‑sequential requests */
    if ((size_t)n >= len) {
        size_t new_cap = (cap ? cap : 2);
        while (new_cap <= (size_t)n + 1) new_cap *= 2;

        /* realloc with the proper cast for C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – terminate silently as required */
            abort();
        }
        cache = tmp;
        cap   = new_cap;

        /* compute missing values up to n */
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i <= 1) {
                cache[i] = (seq_t)1;
                continue;
            }
            idx_t i64 = (idx_t)i;
            seq_t term1 = (seq_t)9 *
                          ((seq_t)287 * (seq_t)i64 - (seq_t)720) *
                          cache[i - 1];
            seq_t term2 = (seq_t)1458 *
                          ((seq_t)2 * (seq_t)i64 - (seq_t)3) *
                          cache[i - 2];
            seq_t num   = term1 + term2;
            seq_t denom = (seq_t)8 * ((seq_t)i64 - (seq_t)1);
            cache[i] = num / denom;
        }
        len = (size_t)n + 1;
    }

    return cache[n];
}
