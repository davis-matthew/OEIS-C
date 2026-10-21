/*
 * A076726  a(n) = Sum_{k>=0} k^n/2^k.
 * https://oeis.org/A076726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(n) = \sum_{k=0}^{n-1} binom(n,k) * a(k) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;   /* current number of stored terms   */
    static idx_t cache_cap   = 0;   /* allocated capacity of cache      */

    /* initialise cache with a(0) */
    if (cache_cap == 0) {
        cache_cap = 4;            /* arbitrary small capacity */
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)2;      /* a(0) = 2 */
        cache_size = 1;
    }

    /* grow cache until it contains term n */
    while (cache_size <= n) {
        idx_t m = cache_size;          /* compute a(m) */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* binom(m,0) */

        /* accumulate sum_{k=0}^{m-1} binom(m,k) * a(k) */
        for (idx_t k = 0; k < m; ++k) {
            sum += binom * cache[(size_t)k];

            /* update binomial coefficient: binom(m,k+1) = binom(m,k) * (m-k) / (k+1) */
            binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
        }

        /* append new term */
        if (cache_size >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size] = sum;
        ++cache_size;
    }

    return cache[(size_t)n];
}
