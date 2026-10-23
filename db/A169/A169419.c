/*
 * A169419  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0: a(n) = p(n) + 20*∑_{k=1}^{min(31,n)} a(n-k) –
 *            (n≥32 ? 210*a(n-32) : 0) where p(0)=1, p(1..31)=2, p(32)=1,
 *            p(n≥33)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* memoized values */
    static idx_t cache_len  = 0;      /* number of values stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Grow capacity if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t p;
        if (i == 0)
            p = (seq_t)1;
        else if (i <= 31)
            p = (seq_t)2;
        else if (i == 32)
            p = (seq_t)1;
        else
            p = (seq_t)0;

        /* Sum of the previous up to 31 terms */
        seq_t sum_prev = (seq_t)0;
        for (idx_t k = 1; k <= 31 && i >= k; ++k)
            sum_prev += cache[i - k];

        seq_t val = p + (seq_t)20 * sum_prev;
        if (i >= 32)
            val -= (seq_t)210 * cache[i - 32];

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
