/*
 * A167174  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167174: a(n) satisfies a linear recurrence derived from the
   generating function
       P(t)/Q(t),  P(t)=1+2t+2t^2+…+2t^13+t^14,
                     Q(t)=1-21t-21t^2-…-21t^13+231t^14.
   For n≥0:
       a(n) = 21*a(n-1)+21*a(n-2)+…+21*a(n-13) - 231*a(n-14) + p(n),
   where p(0)=1, p(14)=1 and p(k)=2 for 1≤k≤13.
   The first computed term is a(0)=1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;      /* storage for a[0] … a[computed-1]    */
    static size_t cap   = 0;         /* allocated capacity of cache           */
    static size_t computed = 0;      /* number of terms already computed      */

    /* Coefficients of P(t) for 0 ≤ k ≤ 14  (constant term first)  */
    static const seq_t P[15] = {
        1ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL,
        2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 1ULL
    };

    /* Ensure cache has space for [0 … n] */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *newcache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* If realloc fails, newcache is NULL and we should abort,
           but for the purpose of the library we assume enough memory. */
        cache = newcache;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = 0;

        /* Add 21 * a(i-k) for k = 1 … 13, provided i-k >= 0 */
        for (idx_t k = 1; k <= 13 && i >= k; ++k)
            sum += (seq_t)21 * cache[i - k];

        /* Subtract 231 * a(i-14) if i-14 ≥ 0 */
        if (i >= 14)
            sum -= (seq_t)231 * cache[i - 14];

        /* Add the inhomogeneous part p(i) if i ≤ 14 */
        if (i <= 14)
            sum += P[i];

        cache[i] = sum;
    }

    computed = (size_t)n + 1;   /* now we have computed up to n */
    return cache[n];
}
