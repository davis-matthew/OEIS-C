/*
 * A126444  a(n) = Sum_{k=0..n-1} C(n-1,k)*a(k)*a(n-1-k)*2^k for n>0, with
 *          a(0)=1.
 * https://oeis.org/A126444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, for n>0 a(n) = Sum_{k=0..n-1} C(n-1,k) * a(k) * a(n-1-k)
 *            * 2^k
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;
    static seq_t *pow2 = NULL;
    static size_t pow2_cap = 0;

    /* initialise the tables on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;      /* a(0) = 1 */
    }
    if (!pow2) {
        pow2_cap = 1;
        pow2 = (seq_t *)malloc(pow2_cap * sizeof(seq_t));
        if (!pow2) return (seq_t)0;
        pow2[0] = (seq_t)1;      /* 2^0 = 1 */
    }

    /* grow cache if n is outside current bounds */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap*2+1 : 2;
        if ((size_t)n >= new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }
    /* grow pow2 table if needed */
    if ((size_t)n >= pow2_cap) {
        size_t new_cap = pow2_cap ? pow2_cap*2+1 : 2;
        if ((size_t)n >= new_cap) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        pow2 = tmp;
        for (size_t i = pow2_cap; i < new_cap; ++i)
            pow2[i] = pow2[i-1] * (seq_t)2;
        pow2_cap = new_cap;
    }

    /* compute any missing values up to n */
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            idx_t m = i - 1;              /* n-1 */
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;       /* C(m,0) */

            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = cache[k] * cache[m - k];
                term = term * pow2[k];
                term = term * binom;
                sum = sum + term;

                if (k < m) {             /* prepare next binomial */
                    binom = binom * (seq_t)(m - k);
                    binom = binom / (seq_t)(k + 1);
                }
            }
            cache[i] = sum;
        }
        cache_last = n;
    }

    return cache[n];
}
