/*
 * A025955  Expansion of 1/((1-2*x)*(1-3*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A025955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1)-221*a(n-2)+676*a(n-3)-660*a(n-4) Initial terms:
 *            a(0)=1, a(1)=26, a(2)=455, a(3)=6760
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoization table */
    static idx_t cache_size = 0;       /* number of stored values */

    /* -- initialise cache on first call --------------------------------- */
    if (cache == NULL) {
        size_t init_cap = 4;                                  /* hold a(0)..a(3)     */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();                                  /* impossible in tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)455;
        cache[3] = (seq_t)6760;
        cache_size = 4;
    }

    /* -- grow cache if necessary ---------------------------------------- */
    if (n >= cache_size) {
        size_t needed = (size_t)n + 1;                 /* allocate up to n       */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                             /* avoid leak             */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence only for i >= 4   */
            seq_t v1 = cache[i-1] * (seq_t)26;
            seq_t v2 = cache[i-2] * (seq_t)221;
            seq_t v3 = cache[i-3] * (seq_t)676;
            seq_t v4 = cache[i-4] * (seq_t)660;
            cache[i] = v1 + v3 - v2 - v4;   /* 26*a(n-1)+676*a(n-3)-221*a(n-2)-660*a(n-4) */
        }
        cache_size = (idx_t)needed;
    }

    return cache[n];
}
