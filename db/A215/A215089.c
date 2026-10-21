/*
 * A215089  a(n)=Sum{d(i)*6^i: i=0,1,...,m}, where Sum{d(i)*2^i: i=0,1,...,m}
 *          is the base 2 representation of n.
 * https://oeis.org/A215089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   a(n) = 2*a(n/6)          if n % 6 == 0,
   a(n) = a(n-1) + 1        otherwise.   */
/* The sequence is: 0,1,2,3,4,5,2,3,4,5,6,7,4,5,6,7,8,9,6,7,8,9,10,11,… */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* pointer to array of seq_t values */
    static idx_t  cached_n = 0;     /* number of stored terms */

    /* Allocate or grow the cache up to n+1 elements */
    if (n >= cached_n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure – acceptable in test harness */
        cache = tmp;

        /* Compute new entries iteratively */
        for (idx_t i = cached_n; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if ((i % 6) == 0) {
                cache[i] = cache[i / 6] << 1; /* 2 * a(i/6) */
            } else {
                cache[i] = cache[i - 1] + 1;
            }
        }
        cached_n = new_cap;
    }

    return cache[n];
}
