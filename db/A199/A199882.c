/*
 * A199882  E.g.f. A(x) satisfies differential equation A'''(x)=A(x)+A(x)^2,
 *          A(0)=0, A'(0)=1, A''(0)=1, A'''(0)=1.
 * https://oeis.org/A199882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-3) + \sum_{i=1}^{n-4} a(i)*a(n-3-i)*C(n-3,i),
 *            a(1)=a(2)=a(3)=1 where C(m,i) = binomial coefficient. This is
 *            equivalent to the OEIS definition using b(n)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  max_cached = 0; /* highest index for which cache[k] is valid (>=1) */
    static idx_t  cache_cap = 0;  /* allocated size (>=n+1) */

    /* Ensure enough capacity */
    if (n >= cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : n + 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                       /* allocation failed, caller must handle */
        /* zero new entries */
        for (idx_t i = cache_cap; i <= n; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values */
    for (idx_t k = max_cached + 1; k <= n; ++k) {
        if (k <= 3) {               /* a(1)=a(2)=a(3)=1 */
            cache[k] = 1;
        } else {
            idx_t m = k - 3;        /* denominator in C(m,i) */
            seq_t sum = cache[k - 3];   /* add the a(k-3) term */

            seq_t binom = 1;                 /* C(m,0) = 1 */
            for (idx_t i = 1; i <= m - 1; ++i) {
                /* C(m,i) = C(m,i-1) * (m - i + 1) / i */
                seq_t num = (seq_t)(m - (i - 1));
                seq_t den = (seq_t)i;
                binom = binom * num / den;

                seq_t term = cache[i] * cache[k - 3 - i] * binom;
                sum += term;
            }
            cache[k] = sum;
        }
    }
    max_cached = n;
    return cache[n];
}
