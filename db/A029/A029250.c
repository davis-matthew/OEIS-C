/*
 * A029250  Expansion of 1/((1-x^3)*(1-x^4)*(1-x^5)*(1-x^12)).
 * https://oeis.org/A029250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into 3,4,5,12. * Recurrence:
 *            a(n) = a(n-3)+a(n-4)+a(n-5)-a(n-7)-a(n-8)-a(n-9) *
 *            +2*a(n-12)-a(n-15)-a(n-16)-a(n-17)+a(n-19)+a(n-20)+a(n-21)-a(n-24),
 *            * with a(0)=1, a(1)=0, a(2)=0
 * Range:     u8 n <= 91, u16 n <= 644, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* memoized values */
    static idx_t cache_len = 0;     /* current allocated size (cache_len entries, indices 0 .. cache_len-1) */

    /* enlarge cache if necessary */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;                     /* need indices 0..n inclusive */

        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;

        /* initialise new entries */
        for (idx_t i = old_len; i < new_len; ++i)
            cache[i] = (seq_t)0;

        /* set base values if they haven't been set */
        if (old_len <= 3) {
            if (0 >= 0) cache[0] = (seq_t)1;
            if (1 >= 0 && cache_len > 1) cache[1] = (seq_t)0;
            if (2 >= 0 && cache_len > 2) cache[2] = (seq_t)0;
        }

        /* compute values from the first missing index up to n */
        idx_t start = (old_len > 2 ? old_len : 3);  /* indices 0..2 already initialised */
        for (idx_t i = start; i <= n; ++i) {
            seq_t val = (seq_t)0;

            if (i >= 3) val += cache[i - 3];
            if (i >= 4) val += cache[i - 4];
            if (i >= 5) val += cache[i - 5];
            if (i >= 7) val -= cache[i - 7];
            if (i >= 8) val -= cache[i - 8];
            if (i >= 9) val -= cache[i - 9];
            if (i >= 12) { val += cache[i - 12]; val += cache[i - 12]; }   /* +2*a[n-12] */
            if (i >= 15) val -= cache[i - 15];
            if (i >= 16) val -= cache[i - 16];
            if (i >= 17) val -= cache[i - 17];
            if (i >= 19) val += cache[i - 19];
            if (i >= 20) val += cache[i - 20];
            if (i >= 21) val += cache[i - 21];
            if (i >= 24) val -= cache[i - 24];

            cache[i] = val;
        }
    }
    return cache[n];
}
