/*
 * A168957  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the rational generating function G(t) = (t^22 + 2
 *            t^21 + … + 2 t + 1) / (741 t^22 – 38 t^21 – … – 38 t + 1). The
 *            recurrence for the coefficients a(n) is a(0) = 1 for 1 ≤ n ≤ 21
 *            : a(n) = 38 · (a(n‑1)+…+a(0)) + 2 for n = 22 : a(22) = 38 ·
 *            (a(21)+…+a(1)) – 741·a(0) + 1 for n ≥ 23 : a(...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>       /* for realloc */

seq_t a(idx_t n)
{
    /* Cached values – initially empty */
    static seq_t *cache    = NULL;     /* array of seq_t */
    static size_t  cache_len = 0;      /* number of computed terms */
    static size_t  cache_cap = 0;      /* allocated capacity */

    /* Ensure capacity for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = ((size_t)n + 1) * 2;  /* grow factor 2 to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t value;

        if (i == 0) {
            /* Base value */
            value = (seq_t)1;
        } else {
            /* Constant term from numerator */
            if (i <= 21)          value = (seq_t)2;
            else if (i == 22)     value = (seq_t)1;
            else                  value = (seq_t)0;

            /* 38 * sum_{k=1}^{min(21,i)} a(i-k) */
            idx_t kmax = (i < 21) ? i : 21;
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= kmax; ++k) {
                sum += cache[i - k];
            }
            value += (seq_t)38 * sum;

            /* -741 * a(i-22) for i ≥ 22 */
            if (i >= 22) {
                value -= (seq_t)741 * cache[i - 22];
            }
        }

        cache[i] = value;
    }

    /* The sequence length now contains at least n+1 elements */
    if (cache_len <= (size_t)n) cache_len = n + 1;

    return cache[n];
}
