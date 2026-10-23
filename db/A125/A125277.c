/*
 * A125277  Eigensequence of triangle A110616: a(n) = Sum_{k=0..n-1}
 *          A110616(n-1,k)*a(k) for n>0 with a(0)=1.
 * https://oeis.org/A125277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n>0 a(n) = Sum_{k=0..n-1} a(k) * C(3*n-2*k-2,
 *            n-k-1) * (k+1) / (3*n-2*k-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* multiplicative binomial coefficient: C(a,b) */
static seq_t binom_small(idx_t a, idx_t b)
{
    if (b > a - b) b = a - b;
    seq_t r = 1;
    for (idx_t i = 1; i <= b; ++i) {
        r *= (seq_t)(a - b + i);
        r /= (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ path: use std::vector for automatic construction/destruction */
    static std::vector<seq_t> cache{1};
    idx_t cur_size = cache.size();
    if (n >= cur_size) {
        cache.resize(n + 1);
        for (idx_t curr = cur_size; curr <= n; ++curr) {
            seq_t sum = 0;
            /* initial binomial for this row */
            seq_t t = binom_small(3 * curr - 2, curr - 1);
            idx_t d = 3 * curr - 2;
            for (idx_t k = 0; k < curr; ++k) {
                idx_t r = curr - k - 1;
                /* weight b_{curr,k} = t * (k+1) / d */
                seq_t val = t * (seq_t)(k + 1);
                val /= (seq_t)d;
                sum += cache[k] * val;
                if (k < curr - 1) {
                    seq_t num = t * (seq_t)r * (seq_t)(d - r);
                    seq_t den = (seq_t)d * (seq_t)(d - 1);
                    t = num / den;
                    d -= 2;
                }
            }
            cache[curr] = sum;
        }
    }
    return cache[n];
#else
    /* C path: use manual dynamic array */
    static idx_t cache_len = 0;
    static seq_t *cache = NULL;
    if (!cache) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;
        cache_len = 1;
    }
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) { /* handle allocation failure */
            return 0;
        }
        cache = new_cache;
        for (idx_t curr = cache_len; curr <= n; ++curr) {
            seq_t sum = 0;
            seq_t t = binom_small(3 * curr - 2, curr - 1);
            idx_t d = 3 * curr - 2;
            for (idx_t k = 0; k < curr; ++k) {
                idx_t r = curr - k - 1;
                seq_t val = t * (seq_t)(k + 1);
                val /= (seq_t)d;
                sum += cache[k] * val;
                if (k < curr - 1) {
                    seq_t num = t * (seq_t)r * (seq_t)(d - r);
                    seq_t den = (seq_t)d * (seq_t)(d - 1);
                    t = num / den;
                    d -= 2;
                }
            }
            cache[curr] = sum;
        }
        cache_len = n + 1;
    }
    return cache[n];
#endif
}
