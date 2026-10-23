/*
 * A170081  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence a(n) = 22*∑_{i=1}^{36} a(n-i)
 *            – 253*a(n-37) for n ≥ 37 with initial values a(0) = 1, a(n) = 2
 *            + 22*∑_{j=0}^{n-1} a(j) for 1 ≤ n ≤ 36. (These initial terms
 *            come from the generating function numerator 1, 2t, …, 2t^36,
 *            t^37.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache  = NULL;  /* array of terms */
    static idx_t  len    = 0;     /* number of terms stored (size of cache) */
    static idx_t  cap    = 0;     /* allocated capacity of cache */

    /* ------------------------------------------------------------
       Initialise cache with the first 37 terms using the
       special initial recurrence derived from the numerator.
       ------------------------------------------------------------ */
    if (len == 0) {
        /* we need exactly 37 terms: a[0] … a[36] */
        len = 37;
        cap = 37;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – unlikely */

        /* a(0) = 1 */
        cache[0] = (seq_t)1;
        /* running sum S(n-1) = a[0] + ... + a[n-1] */
        seq_t cumulative = cache[0];

        for (idx_t i = 1; i < len; ++i) {
            /* a[i] = 2 + 22 * cumulative, 1 ≤ i ≤ 36   */
            cache[i] = (seq_t)2 + (seq_t)22 * cumulative;
            cumulative += cache[i];
        }
    }

    /* ------------------------------------------------------------
       Return quickly if the requested index is already cached.
       ------------------------------------------------------------ */
    if (n < len)
        return cache[n];

    /* ------------------------------------------------------------
       Ensure the cache has enough capacity for indices 0 … n.
       ------------------------------------------------------------ */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap : 37;
        while (new_cap <= (size_t)n)
            new_cap *= 2;               /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – unlikely */
        cache = tmp;
        cap   = new_cap;
    }

    /* ------------------------------------------------------------
       Compute the missing terms up to index n using the 37‑term
       linear recurrence.
       ------------------------------------------------------------ */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 37) {
            /* This case cannot happen – a[0]..a[36] are already known */
            continue;
        }
        seq_t sum_big = 0;
        for (idx_t k = 1; k <= 36; ++k)
            sum_big += cache[i - k];

        cache[i] = sum_big * (seq_t)22 - cache[i - 37] * (seq_t)253;
    }

    len = n + 1;        /* we now have terms up to n */
    return cache[n];
}
