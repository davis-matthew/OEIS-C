/*
 * A016430  Coordination sequence T1 for Zeolite Code OSI.
 * https://oeis.org/A016430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2*a(n-1) - 3*a(n-2) + 5*a(n-3) - 6*a(n-4) + 7*a(n-5) *
 *            - 7*a(n-6) + 6*a(n-7) - 5*a(n-8) + 3*a(n-9) - 2*a(n-10) +
 *            a(n-11) * for n >= 14. Initial values: * a(0)=1, a(1)=4,
 *            a(2)=10, a(3)=21, a(4)=37, a(5)=57, a(6)=82, a(7)=111, *
 *            a(8)=145, a(9)=189, a(10)=236, a(11)=280, a(12)=332, a(13)=392
 * Range:     u8 n <= 10, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Initial values a(0) … a(13) */
    static const seq_t init[14] = {
        1, 4, 10, 21, 37, 57, 82, 111, 145, 189,
        236, 280, 332, 392
    };
    /* Recurrence coefficients for a(n) = Σ c_k * a(n-k-1)  */
    static const int coeff[11] = {
        2, -3, 5, -6, 7, -7, 6, -5, 3, -2, 1
    };

    /* Cached sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* First call: allocate and store initial terms */
    if (!cache) {
        cache_cap = 14;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 14; ++i)
            cache[i] = init[i];
        cache_len = 14;
    }

    /* Return the value if it is already cached */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to hold terms up to n */
    size_t need_len = (size_t)n + 1;
    if (need_len > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < need_len)
            new_cap = need_len;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new terms iteratively */
    for (size_t i = cache_len; i < need_len; ++i) {
        seq_t pos_sum = 0;   /* sum of positive coefficient terms */
        seq_t neg_sum = 0;   /* sum of absolute values of negative coeff. terms */

        for (int k = 0; k < 11; ++k) {
            seq_t term = cache[i - 1 - k];
            int c = coeff[k];
            if (c > 0) {
                pos_sum += (seq_t)c * term;
            } else {
                neg_sum += (seq_t)(-c) * term;
            }
        }
        cache[i] = pos_sum - neg_sum;
    }

    cache_len = need_len;
    return cache[(size_t)n];
}
