/*
 * A167659  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A167659 :  Number of reduced words of length n in a Coxeter group
 *
 *  a(0) = 1
 *  For n >= 1
 *      a(n) = p(n) + 8·(a(n-1)+…+a(n-14)) – 36·a(n-15)
 *  where p(n) = 1  if n==0 or n==15
 *         p(n) = 2  for 1 ≤ n ≤ 14
 *         p(n) = 0  otherwise
 *  The recurrence follows from the rational generating function
 *  given in the OEIS entry.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

seq_t a(idx_t n)
{
    /* dynamic cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of values stored (up to cache_len-1) */
    static size_t cache_cap = 0;  /* allocated size */

    if (n < cache_len)          /* value already computed */
        return cache[n];

    /* grow cache */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    } else {
        while (cache_cap <= (size_t)n) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t p;
        if (i == 0 || i == 15)      p = (seq_t)1;
        else if (i <= 15)           p = (seq_t)2;
        else                        p = (seq_t)0;

        seq_t sum = 0;
        for (idx_t k = 1; k <= 14 && k <= i; ++k) {
            sum += (seq_t)8 * cache[i - k];
        }
        if (i >= 15)
            sum -= (seq_t)36 * cache[i - 15];

        cache[i] = p + sum;
    }

    cache_len = n + 1;   /* now up to n is stored */
    return cache[n];
}
