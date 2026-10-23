/*
 * A170153  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46*(a(n-1)+a(n-2)+...+a(n-37)) - 1081*a(n-38) for n>=38,
 *            * with a(0)=1 and a(1..37) determined by the rational function *
 *            (t^38 + 2*t^37 + ... + 2*t + 1)/(1081*t^38 - 46*t^37 - ... -
 *            46*t + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;  /* array of computed values */
    static size_t cap        = 0;     /* allocated capacity */
    static idx_t max_computed = 0;    /* highest index computed */

    /* ensure cache has room for up to index n */
    if (cache == NULL) {
        /* initial allocation */
        cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        max_computed = 0;
    }
    else if (n >= cap) {
        /* grow cache (doubling strategy) */
        size_t new_cap = cap ? cap * 2 : 1;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* compute terms up to index n if needed */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k <= 37) { /* initial terms (k>=1) */
            seq_t sum = 0;
            for (idx_t j = 1; j <= k; ++j)
                sum += cache[k - j];
            cache[k] = (seq_t)((seq_t)2 + (seq_t)46 * sum);
        } else { /* recurrence for k >= 38 */
            seq_t sum = 0;
            for (idx_t j = 1; j <= 37; ++j)
                sum += cache[k - j];
            cache[k] = (seq_t)46 * sum - (seq_t)1081 * cache[k - 38];
        }
    }
    max_computed = n;
    return cache[n];
}
