/*
 * A170126  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*( a(n-1)+a(n-2)+...+a(n-37) ) - 190*a(n-38) with
 *            initial terms a(0)=1, a(1)=21, a(2)=420, a(3)=8400, a(4)=168000,
 *            a(5)=3360000, … (derived from the given rational generating
 *            function). The values for n < 38 are obtained by adding the
 *            corresponding numerator coefficient (p(n)=1 for n=0 o...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i) for i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static idx_t cache_cap = 0;   /* allocated size of cache */

    /* increase capacity if necessary */
    if (n >= cache_cap)
    {
        size_t new_cap = (size_t)n + 1;
        if ((size_t)cache_cap * 2 > new_cap)
            new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t sum = 0;
        /* sum of previous 37 terms */
        for (idx_t k = 1; k <= 37; ++k)
        {
            if (i >= k)
                sum += cache[i - k];
        }

        seq_t val = (seq_t)19 * sum;          /* 19 * sum_{k=1}^{37} a[i-k] */

        if (i >= 38)
            val -= (seq_t)190 * cache[i - 38]; /* -190 * a[i-38] */

        /* add numerator contribution p(i) */
        seq_t p = 0;
        if (i == 0 || i == 38)
            p = (seq_t)1;
        else if (i <= 37)
            p = (seq_t)2;
        val += p;

        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
