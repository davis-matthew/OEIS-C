/*
 * A212988  Number of (w,x,y) with all terms in {0,...,n} and 4*w = x+y.
 * https://oeis.org/A212988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)-a(n-2)+a(n-4)-2*a(n-5)+a(n-6) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=7, a(5)=9, a(6)=12
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_max = 0;
    static int initialized = 0;

    /* initialise cache with base values (a(0) .. a(6)) */
    if (!initialized) {
        /* start with a reasonable capacity */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* fill base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)9;
        cache[6] = (seq_t)12;
        computed_max = 6;
        initialized = 1;
    }

    /* grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while ((size_t)n >= new_cap)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute up to n if necessary */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i <= 6) {
                /* already initialised */
                continue;
            }
            /* recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) + a(n-6) */
            seq_t term1 = (seq_t)2 * cache[i - 1];
            seq_t term2 = cache[i - 2];
            seq_t term3 = cache[i - 4];
            seq_t term4 = (seq_t)2 * cache[i - 5];
            seq_t term5 = cache[i - 6];
            cache[i] = term1 - term2 + term3 - term4 + term5;
        }
        computed_max = n;
    }

    return cache[n];
}
