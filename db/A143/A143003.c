/*
 * A143003  a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+5)*a(n) - n^6*a(n-1).
 * https://oeis.org/A143003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n+1) = (2*n+1)*(n^2+n+5)*a(n) - n^6*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization of sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_n = 0; /* highest index already computed */

    /* Initial allocation for a(0) and a(1) */
    if (cache == NULL) {
        size_t init_size = 2;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed, return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_n = 1;
    }

    /* Return cached value if already computed */
    if (n <= cache_n) {
        return cache[n];
    }

    /* Resize cache to hold values up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;   /* allocation failed */
    cache = tmp;

    /* Compute missing entries using recurrence */
    for (idx_t k = cache_n + 1; k <= n; ++k) {
        /* a(k) depends on a(k-1) and a(k-2) */
        seq_t km1 = (seq_t)(k - 1);               /* k-1 as seq_t */
        seq_t km1_sq = km1 * km1;                 /* (k-1)^2 */

        /* coeff1 = 2*k - 1 */
        seq_t coeff1 = (seq_t)(2 * k - 1);

        /* coeff2 = (k-1)^2 + (k-1) + 5 */
        seq_t coeff2 = km1_sq + km1 + (seq_t)5;

        /* term1 = coeff1 * coeff2 * a(k-1) */
        seq_t term1 = coeff1 * coeff2 * cache[k - 1];

        /* km1^6 = ((k-1)^2)^3 = km1_sq^3 */
        seq_t km1_pow6 = km1_sq * km1_sq * km1_sq;

        /* term2 = km1^6 * a(k-2) */
        seq_t term2 = km1_pow6 * cache[k - 2];

        cache[k] = term1 - term2;
    }

    cache_n = n;
    return cache[n];
}
