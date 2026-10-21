/*
 * A223056  Number of n X 2 0..3 arrays with all horizontally or vertically
 *          connected equal values in a straight line, and new values 0..3
 *          introduced in row major order.
 * https://oeis.org/A223056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A223056: Number of n × 2 0..3 arrays with all horizontally or vertically
   connected equal values in a straight line, and new values 0..3 introduced in row
   major order.  Recurrence (valid for n>5):
     a(n) = 18*a(n-1) - 71*a(n-2) + 102*a(n-3) - 48*a(n-4)
   Initial values:
     a(1) = 2, a(2) = 10, a(3) = 114, a(4) = 1473, a(5) = 19360
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* out of bounds, never used */

    static seq_t *cache = NULL;     /* memoized values, indexed from 1 */
    static size_t cap   = 0;        /* allocated capacity (number of slots) */
    static size_t last  = 0;        /* highest index computed so far */

    /* initial allocation and seed values */
    if (cache == NULL) {
        cap = 32;                    /* start with a reasonable size */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)0;         /* unused, keep index starting at 1 */

        /* seed the first five terms */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)114;
        cache[4] = (seq_t)1473;
        cache[5] = (seq_t)19360;
        last = 5;
    }

    /* ensure capacity for index n */
    if (n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n)
            newcap <<= 1;           /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            exit(1);
        cache = tmp;
        cap = newcap;
    }

    /* if already computed, return */
    if (n <= last)
        return cache[n];

    /* compute missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t v = (seq_t)18 * cache[i - 1]
                - (seq_t)71 * cache[i - 2]
                + (seq_t)102 * cache[i - 3]
                - (seq_t)48 * cache[i - 4];
        cache[i] = v;
    }
    last = n;

    return cache[n];
}
