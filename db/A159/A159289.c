/*
 * A159289  a(n+1) = 5*a(n) - 2*a(n-1), a(0)=5, a(1)=21.
 * https://oeis.org/A159289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: a(0) = 5, a(1) = 21,
                a(n) = 5*a(n-1) - 2*a(n-2)   for n >= 2
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms           */
    static idx_t cap   = 0;        /* capacity of cache array  */

    /* Base cases */
    if (n == 0u) return (seq_t)5;
    if (n == 1u) return (seq_t)21;

    /* If we already have enough entries, return the cached value. */
    if (cap > n) return cache[n];

    /* Need to enlarge the cache up to index n (inclusive). */
    idx_t newcap = n + 1u;                     /* number of elements */
    /* Use a C-style cast so the code compiles in C and in C++   */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                 /* allocation failure */

    cache = tmp;
    if (cap == 0u) {                           /* first allocation */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)21;
        cap     = 2u;
    }

    /* Compute new entries iteratively */
    for (idx_t i = cap; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i-1] - (seq_t)2 * cache[i-2];
    }

    cap = newcap;
    return cache[n];
}
