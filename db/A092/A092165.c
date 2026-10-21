/*
 * A092165  Let M = 2 X 2 matrix [ 1 2 / 5 4 ]; a(n) = (1,2) entry of M^n.
 * https://oeis.org/A092165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2); a(0)=0, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;            /* array of values a(0)..a(k) */
    static size_t capacity = 0;            /* allocated array size */
    static idx_t last = 1;                 /* highest index computed */

    /* initialise the cache on first call */
    if (cache == NULL) {
        capacity = 2;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
    }

    if ((size_t)n < last)  /* value already computed */
        return cache[n];

    /* enlarge the cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > capacity) {
        size_t newcap = capacity ? capacity << 1 : 2;
        while (newcap < need) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        /* initialise new slots to zero (they will be overwritten below) */
        for (size_t i = capacity; i < newcap; ++i)
            cache[i] = (seq_t)0;
        capacity = newcap;
    }

    /* compute missing values using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i)
        cache[i] = (seq_t)(5 * cache[i - 1] + 6 * cache[i - 2]);

    last = n;                   /* update highest computed index */
    return cache[n];
}
