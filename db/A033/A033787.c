/*
 * A033787  Product t2(q^d); d | 28, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^n] ∏_{d|28} t2(q^d), where t2(q) = 1 + q + q^3 + q^6 +
 *            q^10 + … = Σ_{m≥0} q^{m(m+1)/2}
 * Range:     u8 n <= 123, u16 n <= 2039, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values up to cache_max_n */
static idx_t cache_max_n = (idx_t)-1;/* maximum n for which cache is valid */

seq_t a(idx_t n)
{
    /* if already cached, return stored value */
    if (cache && n <= cache_max_n)
        return cache[n];

    /* recompute up to n */
    size_t size = (size_t)(n + 1);

    /* allocate two working arrays */
    seq_t *dp1 = NULL;
    seq_t *dp2 = NULL;
#ifdef __cplusplus
    dp1 = new seq_t[size];
    dp2 = new seq_t[size];
    for (size_t i = 0; i < size; ++i) dp1[i] = dp2[i] = 0;
#else
    dp1 = (seq_t *)calloc(size, sizeof(seq_t));
    dp2 = (seq_t *)calloc(size, sizeof(seq_t));
#endif
    if (!dp1 || !dp2) {
        /* Allocation failed – return 0 as safe fallback */
#ifdef __cplusplus
        delete [] dp1;
        delete [] dp2;
#else
        free(dp1);
        free(dp2);
#endif
        return 0;
    }

    dp1[0] = 1; /* base case: coefficient for q^0 is 1 */

    /* divisors of 28 */
    const idx_t divisors[6] = {1, 2, 4, 7, 14, 28};

    for (size_t di = 0; di < 6; ++di) {
        const idx_t d = divisors[di];

        /* zero the second array */
        for (size_t i = 0; i < size; ++i)
            dp2[i] = 0;

        /* iterate over all m such that e = d * m*(m+1)/2 ≤ n */
        for (idx_t m = 0;; ++m) {
            idx_t tri = m * (m + 1) / 2;
            idx_t e = d * tri;
            if (e > n)
                break;

            /* convolution step: add this exponent to all existing sums */
            for (idx_t x = 0; x <= n; ++x) {
                idx_t y = x + e;
                if (y > n)
                    break;
                dp2[y] += dp1[x];
            }
        }

        /* swap dp1 and dp2 for the next divisor */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    /* store result into cache */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[size];
#else
    seq_t *new_cache = (seq_t *)calloc(size, sizeof(seq_t));
#endif
    if (!new_cache) {
        /* Allocation failed – clean up and return 0 */
#ifdef __cplusplus
        delete [] dp1;
        delete [] dp2;
#else
        free(dp1);
        free(dp2);
#endif
        return 0;
    }
    for (size_t i = 0; i < size; ++i)
        new_cache[i] = dp1[i];

    /* release old cache and working arrays */
    if (cache) {
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }
#ifdef __cplusplus
    delete [] dp1;
    delete [] dp2;
#else
    free(dp1);
    free(dp2);
#endif

    cache = new_cache;
    cache_max_n = n;

    return cache[n];
}
