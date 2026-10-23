/*
 * A168828  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*(a(n-1)+…+a(n-19)) – 15*a(n-20) + p(n) where
 *            p(0)=p(20)=1 and p(k)=2 for 1≤k≤19. For n>20 the term p(n) is
 *            zero
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary‑order calls */
    static seq_t *cache = NULL;
    static idx_t computed_up_to = 0;   /* last index that is valid in cache   */
    static size_t cache_capacity = 0;  /* allocated array size (in elements) */

    /* Ensure the cache is large enough and the zeroth term is initialized. */
    if (cache == NULL) {
        cache_capacity = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 1;          /* a(0)=1 from the recurrence (p(0)=1) */
        computed_up_to = 0;
    } else if ((size_t)n + 1 > cache_capacity) {
        cache_capacity = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t term = 0;

        /* 5 * (a(i-1)+...+a(i-19)) */
        for (idx_t j = 1; j <= 19 && i >= j; ++j) {
            term += (seq_t)5 * cache[i - j];
        }

        /* –15 * a(i-20)   */
        if (i >= 20) {
            term -= (seq_t)15 * cache[i - 20];
        }

        /* + p(i) for i ≤ 20   */
        if (i <= 20) {
            term += (i == 0 || i == 20) ? (seq_t)1 : (seq_t)2;
        }

        cache[i] = term;
    }

    computed_up_to = n;
    return cache[n];
}
