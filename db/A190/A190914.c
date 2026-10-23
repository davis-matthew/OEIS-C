/*
 * A190914  Expansion of ( 5-9*x^2-2*x^3 ) / ( (1+x-x^2)*(1-x-x^2-x^3) ).
 * https://oeis.org/A190914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) + a(n-3) - a(n-5); Initial values: a(0)=5,
 *            a(1)=0, a(2)=6, a(3)=3, a(4)=18
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;     /* cached terms */
    static idx_t  cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* Ensure cache can hold term n */
    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            /* grow capacity (at least double or enough for needed) */
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)5;
            } else if (i == 1) {
                cache[i] = (seq_t)0;
            } else if (i == 2) {
                cache[i] = (seq_t)6;
            } else if (i == 3) {
                cache[i] = (seq_t)3;
            } else if (i == 4) {
                cache[i] = (seq_t)18;
            } else {
                /* recurrence for i >= 5: a(i) = 3*a(i-2) + a(i-3) - a(i-5) */
                cache[i] = ((seq_t)3 * cache[i - 2]) + cache[i - 3] - cache[i - 5];
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
