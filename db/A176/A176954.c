/*
 * A176954  a(n+1) = 1 + Sum_{p=0..n} a(p)*a(n-p)+k for n>=1, with here
 *          a(0)=1, a(1)=2, k=-1 and l=-1.
 * https://oeis.org/A176954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=1, a(3)=2, a(4)=3, a(5)=9, and for n>=6
 *            (n+1)*a(n)+(2-7*n)*a(n-1)+(11*n-13)*a(n-2)+(11*n-46)*a(n-3)+4*(29-7*n)*a(n-4)+12*(n-5)*a(n-5)=0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed; caller should handle */
        /* Initialize known terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)3;
        cache[5] = (seq_t)9;
        cache_len = 6;
    }

    /* Grow the cache to accommodate n */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= cache_cap) new_cap = cache_cap + 256; /* avoid overflow */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* For indices 0..5 we already have the values */
            if (i <= 5) {
                continue;
            }

            int iv = (int)i; /* n is small enough to fit in int for our coefficients */
            seq_t pos = 0;
            seq_t neg = 0;

            int coeff1 = 2 - 7 * iv;
            int coeff2 = 11 * iv - 13;
            int coeff3 = 11 * iv - 46;
            int coeff4 = 4 * (29 - 7 * iv);
            int coeff5 = 12 * (iv - 5);

            /* term a[i-1] */
            if (coeff1 > 0)
                pos += (seq_t)coeff1 * cache[i - 1];
            else if (coeff1 < 0)
                neg += (seq_t)(-coeff1) * cache[i - 1];

            /* term a[i-2] */
            if (coeff2 > 0)
                pos += (seq_t)coeff2 * cache[i - 2];
            else if (coeff2 < 0)
                neg += (seq_t)(-coeff2) * cache[i - 2];

            /* term a[i-3] */
            if (coeff3 > 0)
                pos += (seq_t)coeff3 * cache[i - 3];
            else if (coeff3 < 0)
                neg += (seq_t)(-coeff3) * cache[i - 3];

            /* term a[i-4] */
            if (coeff4 > 0)
                pos += (seq_t)coeff4 * cache[i - 4];
            else if (coeff4 < 0)
                neg += (seq_t)(-coeff4) * cache[i - 4];

            /* term a[i-5] */
            if (coeff5 > 0)
                pos += (seq_t)coeff5 * cache[i - 5];
            else if (coeff5 < 0)
                neg += (seq_t)(-coeff5) * cache[i - 5];

            /* Compute absolute sum of other terms: negSum - posSum */
            seq_t abs_sum = (neg > pos) ? (neg - pos) : (pos - neg);
            /* a[i] = abs_sum / (i + 1) */
            cache[i] = abs_sum / ((seq_t)(i + 1));
        }
        cache_len = n + 1;
    }

    return cache[n];
}
