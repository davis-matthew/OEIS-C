/*
 * A017876  Expansion of
 *          1/(1-x^8-x^9-x^10-x^11-x^12-x^13-x^14-x^15-x^16-x^17-x^18).
 * https://oeis.org/A017876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=8}^{18} a(n-k) for n ≥ 8 with initial values a(0)
 *            = 1, a(n) = 0 for 1 ≤ n ≤ 7 (a(8) = 1, a(9) = 1, … a(15) = 1,
 *            a(16) = 2, a(17) = 3, …) This recurrence generates the number of
 *            compositions of n into parts p where 8 ≤ p ≤ 18
 * Range:     u8 n <= 39, u16 n <= 67, u32 n <= 123, u64 n <= 234,
 *            u128 n <= 457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored (max index + 1) */
    static size_t cache_cap = 0;   /* current allocated capacity */

    /* ensure sufficient capacity for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure (unlikely for small n) */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases if not already done */
    if (cache_len == 0) {
        if (cache_cap < 8) {                  /* guard against very small capacity */
            size_t new_cap = 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < 8; ++i) cache[i] = (seq_t)0;
        cache_len = 8;
    }

    /* if the requested index is already known, return it */
    if (n < cache_len) return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 8; k <= 18; ++k) {
            if (i >= (idx_t)k)
                sum += cache[i - k];
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
