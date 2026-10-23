/*
 * A126419  Number of base 32 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n strings over the alphabet {0,1,…,31}
 *            such that the absolute difference between any two adjacent
 *            digits is ≤ 2. The implementation uses dynamic programming over
 *            the 32‑state graph and memoises the sequence values
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* static local state for memoisation */
    static seq_t *a_cache = NULL;   /* array of a(k) values */
    static size_t  cache_cap = 0;   /* allocated capacity of a_cache */
    static idx_t   cache_len = 0;   /* highest n whose a(n) is stored */

    static seq_t  dp_prev[32];      /* DP for current length */
    static seq_t  dp_curr[32];      /* DP for next length */
    static idx_t  dp_len   = 0;     /* current length of dp_prev (start at 1) */

    /* initialisation on first call */
    if (a_cache == NULL) {
        cache_cap = 64;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (a_cache == NULL) {                 /* allocation failure */
            return (seq_t)0;
        }
        a_cache[0] = (seq_t)1;                 /* a(0) */

        /* dp_prev represents length‑1 sequences: one for each digit */
        for (int d = 0; d < 32; ++d) {
            dp_prev[d] = (seq_t)1;
        }
        dp_len = 1;

        /* a(1) = 32 */
        a_cache[1] = (seq_t)32;
        cache_len = 1;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) {
        return a_cache[n];
    }

    /* Ensure cache has room for new values up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {                      /* allocation failure */
            return (seq_t)0;
        }
        a_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute DP iteratively up to the requested n */
    for (idx_t cur = dp_len + 1; cur <= n; ++cur) {
        /* For each digit d, sum contributions from neighbours within ±2 */
        for (int d = 0; d < 32; ++d) {
            seq_t sum = 0;
            int low  = d - 2;
            int high = d + 2;
            if (low < 0)   low = 0;
            if (high > 31) high = 31;
            for (int j = low; j <= high; ++j) {
                sum += dp_prev[j];
            }
            dp_curr[d] = sum;
        }

        /* Sum over all end states to get a(cur) */
        seq_t total = 0;
        for (int d = 0; d < 32; ++d) {
            total += dp_curr[d];
        }
        a_cache[cur] = total;

        /* Prepare dp_prev for next iteration by copying dp_curr */
        for (int d = 0; d < 32; ++d) {
            dp_prev[d] = dp_curr[d];
        }
    }

    dp_len   = n;
    cache_len = n;

    return a_cache[n];
}
