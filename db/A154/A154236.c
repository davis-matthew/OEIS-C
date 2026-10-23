/*
 * A154236  a(n) = ( (5 + sqrt(6))^n - (5 - sqrt(6))^n )/(2*sqrt(6)).
 * https://oeis.org/A154236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (5 + sqrt(6))^n - (5 - sqrt(6))^n )/(2*sqrt(6))
 *            Recurrence: a(0)=0, a(1)=1, a(n) = 10*a(n-1) - 19*a(n-2) for n>1
 *            Offset: 1 (so a(1)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t  cap   = 0;     /* allocated length of cache   */
    static idx_t   next  = 1;     /* first index that has not yet been computed */

    /* Handle n==0 (outside the OEIS offset) */
    if (n == 0)
        return (seq_t)0;

    /* Initialise cache on first call */
    if (!cache) {
        /* Allocate space for indices 0..n (inclusive), at least up to 1 */
        cap = (size_t)(n + 1);
        if (cap < 2)
            cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        next    = 2;           /* next free index */
    }

    /* Grow cache if required */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap)
            new_cap = new_cap ? new_cap * 2 : 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        /* recurrence: a(i) = 10*a(i-1) - 19*a(i-2) */
        cache[i] = cache[i - 1] * (seq_t)10 - cache[i - 2] * (seq_t)19;
    }
    next = n + 1;

    return cache[n];
}
