/*
 * A036080  E.g.f.: exp((exp(p*x)-p-1)/p+exp(x)) for p=10.
 * https://oeis.org/A036080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n) = Σ_{k=1}^{n} binomial(n-1,k-1) * (10^{k-1}+1) *
 *            a(n-k) (n≥1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache[0] = 1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache up to n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* compute missing terms */
        for (size_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;          /* binomial(i-1,0)   */
            seq_t pow10 = (seq_t)1;         /* 10^{k-1} for k=1  */
            seq_t g = pow10 + 1;            /* 10^{k-1} + 1      */

            for (size_t k = 1; k <= i; ++k) {
                size_t idx = i - k;
                seq_t term = binom * g * cache[idx];
                sum += term;
                if (k == i) break;
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)k;
                binom = binom * num / den;          /* exact division   */
                pow10 *= (seq_t)10;
                g = pow10 + 1;
            }
            cache[i] = sum;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
