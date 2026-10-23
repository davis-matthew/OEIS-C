/*
 * A170658  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A170658:
 *   a(0) = 1
 *   For 1 ≤ n ≤ 48:   a(n) = 23 * (a(n-1)+...+a(n-48)) + 2
 *   For n = 49:        a(49) = 23 * (a(48)+...+a(1)) - 276*a(0) + 1
 *   For n > 49:        a(n) = 23 * (a(n-1)+...+a(n-48)) - 276*a(n-49)
 *
 *  This follows from the rational generating function
 *  N(t)/D(t) where N(t)=t^49+2t^48+…+2t+1 and D(t)=1-23t-…-23t^48+276t^49.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0;

    /* Initialise cache with a(0) = 1. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache_last = 0;
    }

    /* If the requested value is already cached, return it. */
    if (n <= cache_last)
        return cache[n];

    /* Extend the cache to hold values up to n. */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute a(i) for all i from cache_last+1 up to n. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum48 = 0;
        idx_t start = (i >= 48) ? (i - 48) : 0;
        for (idx_t k = start; k < i; ++k)
            sum48 += cache[k];

        seq_t val = (seq_t)23 * sum48;

        if (i >= 49)
            val -= (seq_t)276 * cache[i - 49];

        /* Add the N_i term (coefficients of the numerator). */
        seq_t Ni = 0;
        if (i == 0)
            Ni = 1;
        else if (i == 49)
            Ni = 1;
        else if (i <= 48)
            Ni = 2;
        val += Ni;

        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
