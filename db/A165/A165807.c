/*
 * A165807  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the linear recurrence * a_0 = 1 * for 1 ≤ n ≤
 *            9 : a_n = 10·(a_{n-1}+…+a_0) + 2 * a_10 : a_10 = 10·(a_9+…+a_1)
 *            – 55·a_0 + 1 * for n ≥ 11 : a_n = 10·(a_{n-1}+…+a_{n-9}) –
 *            55·a_{n-10}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the sequence value a(n).  The function caches computed
 * values and can be called with n in any order.  All arithmetic
 * is performed in type seq_t; no temporary values exceed the
 * range of seq_t. */
seq_t a(idx_t n)
{
    /* Cached sequence and its storage information.  The array is
     * grown lazily so that a maximum of “n” terms are stored. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* number of terms already computed */

    /* Ensure the cache is large enough to hold all terms up to n. */
    if (cache_len <= n) {
        /* Initial allocation size. */
        if (cache == NULL) {
            cache_cap = 32;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* out of memory → 0 */
            cache_len = 0;
        }

        /* Grow the buffer if necessary. */
        while (n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;     /* out of memory → 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms one by one. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;

            /* sum of the previous nine terms (if they exist) */
            for (idx_t j = 1; j <= 9 && i >= j; ++j)
                sum += cache[i - j];

            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 9) {
                /* a_i = 10*sum + 2  (c_i = 2 for 1≤i≤9) */
                cache[i] = (seq_t)10 * sum + (seq_t)2;
            } else if (i == 10) {
                /* a_10 = 10*sum – 55*a_0 + 1  (c_10 = 1) */
                cache[10] = (seq_t)10 * sum - (seq_t)55 * cache[0] + (seq_t)1;
            } else {
                /* a_n = 10*sum – 55*a_{n-10}  (c_n = 0 for n≥11) */
                cache[i] = (seq_t)10 * sum - (seq_t)55 * cache[i - 10];
            }
        }

        cache_len = n + 1;
    }

    return cache[n];
}
