/*
 * A169080  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=19, a(n)=19·18^(n-1) for 2≤n≤24,
 *            a(n)=17·∑_{k=1}^{24} a(n−k)−153·a(n−25) for n≥25
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The requested function */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity */
    static idx_t   max_n = 0;   /* highest index computed so far */

    /* allocate cache on first call and initialise a(0) */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();     /* allocation failed */
        cache[0] = (seq_t)1;            /* a(0) */
        max_n   = 0;
    }

    /* grow cache if necessary */
    while (n >= cap) {
        size_t newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();      /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute values up to n if not already done */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)19;                /* a(1) */
        } else if (i <= 24) {
            /* a(i) = 19 * 18^(i-1) */
            seq_t p = (seq_t)1;
            for (idx_t k = 1; k < i; ++k) {
                p *= (seq_t)18;
            }
            cache[i] = (seq_t)19 * p;
        } else {
            /* a(i) = 17 * sum_{k=1}^{24} a(i-k) - 153 * a(i-25) */
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 24; ++k) {
                sum += cache[i - k];
            }
            cache[i] = (seq_t)17 * sum - (seq_t)153 * cache[i - 25];
        }
    }
    max_n = n > max_n ? n : max_n;
    return cache[n];
}
