/*
 * A166951  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * The coefficients of the denominator give the linear recurrence
 * a(n) - 10 a(n-1) - ... - 10 a(n-12) + 55 a(n-13) = r(n),
 * where r(n) = 1 (n=0), 2 (1<=n<=12), 1 (n=13), 0 (n>13).
 * Thus
 *   a(n) = 10(a(n-1)+...+a(n-12)) - 55 a(n-13) + r(n).
 * This function computes a(n) using a memoised recurrence.
 */

#include <stdlib.h>

static const seq_t r[14] = {
    1ULL,  /* r(0)  */
    2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 2ULL,
    2ULL, 2ULL, 2ULL, 2ULL, 2ULL, 1ULL   /* r(13) */
};

seq_t a(idx_t n)
{
    /* Cached values of the sequence */
    static seq_t *cache      = NULL;
    static idx_t cache_size = 0;      /* number of elements stored == cache_size */

    if (n < cache_size)            /* already computed */
        return cache[n];

    /* Extend cache if necessary and compute new terms */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure: return 0 as a failure indicator */
        return (seq_t)0;
    }
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        /* sum of last up to 12 terms */
        idx_t limit = (i < 12 ? i : 12);
        seq_t sum12 = 0;
        for (idx_t j = 1; j <= limit; ++j)
            sum12 += cache[i - j];
        seq_t val = sum12 * 10;
        if (i >= 13)
            val -= cache[i - 13] * 55;
        if (i <= 13)        /* r(i) is defined only for i<=13 */
            val += r[i];
        cache[i] = val;
    }
    cache_size = new_size;
    return cache[n];
}
