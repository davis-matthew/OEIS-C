/*
 * A143990  a(n) = n!*A001515(n-1) with a(0) = 1.
 * https://oeis.org/A143990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n-3)*a(n-1) + n*(n-1)*a(n-2) Initial values: a(0) =
 *            1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;	/* array of terms */
    static idx_t  cache_len = 0;	/* capacity of cache (number of slots) */
    static idx_t  comp_len  = 0;	/* how many terms have been computed */

    /* Ensure the cache has enough space for index n */
    if ((size_t)n + 1 > cache_len) {
        /* grow the cache: double its size until it can hold n */
        size_t new_cap = cache_len ? (size_t)cache_len : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */
            return (seq_t)0;
        }

        /* initialize newly allocated elements to zero */
        for (size_t i = cache_len; i < new_cap; ++i)
            tmp[i] = (seq_t)0;

        cache = tmp;
        cache_len = (idx_t)new_cap;
    }

    /* Base cases */
    if (comp_len == 0) {
        cache[0] = (seq_t)1;
        comp_len = 1;
        if (n >= 1) {
            cache[1] = (seq_t)1;
            comp_len = 2;
        }
    } else if (comp_len == 1) {
        cache[1] = (seq_t)1;
        comp_len = 2;
    }

    /* Compute terms from the first missing index up to n */
    for (idx_t k = comp_len; k <= n; ++k) {
        seq_t a_k_minus_1 = cache[k - 1];
        seq_t a_k_minus_2 = cache[k - 2];

        /* term1 = k * (2*k - 3) * a(k-1) */
        seq_t term1 = (seq_t)k * (seq_t)(2 * k - 3) * a_k_minus_1;

        /* term2 = k * (k-1) * a(k-2) */
        seq_t term2 = (seq_t)k * (seq_t)(k - 1) * a_k_minus_2;

        cache[k] = term1 + term2;
    }

    comp_len = n + 1;  /* all terms up to n are now computed */

    return cache[n];
}
