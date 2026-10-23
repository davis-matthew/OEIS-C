/*
 * A048438  Take the first n numbers written in base 6, concatenate them, then
 *          convert from base 6 to base 10.
 * https://oeis.org/A048438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value of the base‑6 number obtained by concatenating the
 *            base‑6 representations of 1,2,…,n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;     /* largest n already computed */
    static size_t cache_cap = 0;   /* current capacity of cache array */

    if (n == 0) return 0;          /* undefined for n<1, but harmless */

    /* Ensure the cache can hold up to n elements */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) return 0;  /* allocation failure → zero (unlikely) */
        cache = new_cache;
        cache_cap = (size_t)n;
    }

    /* Compute values from cached_n+1 through n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* Determine number of base‑6 digits of i */
        idx_t temp = i;
        idx_t d = 0;
        while (temp > 0) {
            temp /= (idx_t)6;
            d++;
        }

        /* Compute 6^d as seq_t */
        seq_t pow6 = (seq_t)1;
        for (idx_t j = 0; j < d; ++j) {
            pow6 *= (seq_t)6;
        }

        /* a(i) = a(i-1)*6^d + i */
        seq_t prev = (i == 1) ? (seq_t)0 : cache[i - 2];
        seq_t cur  = prev * pow6 + (seq_t)i;
        cache[i - 1] = cur;
    }

    cached_n = n;
    return cache[n - 1];
}
