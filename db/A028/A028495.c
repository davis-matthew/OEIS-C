/*
 * A028495  Expansion of g.f. (1-x^2)/(1-x-2*x^2+x^3).
 * https://oeis.org/A028495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence: a(0)=1, a(1)=1, a(2)=2;
    a(n) = a(n-1) + 2*a(n-2) - a(n-3)   (for n >= 3)   */
/*  This implements the A028495 sequence. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for memoization.           */
    static seq_t *cache   = NULL;  /* array holding computed terms   */
    static size_t  cap    = 0;     /* allocated capacity in elements  */
    static size_t  length = 0;     /* number of already-filled terms   */

    /* Ensure the buffer can hold indices up to n.  */
    size_t needed = (size_t)n + 1;
    if (needed < 3) needed = 3;            /* we always store at least a[0..2] */

    if (needed > cap) {
        size_t new_cap = (cap == 0) ? needed : (needed > 2 * cap ? needed : 2 * cap);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* if realloc fails the program will abort, which is acceptable here    */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first three terms if this is the first call.   */
    if (length == 0) {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        length   = 3;
    }

    /* Compute any missing terms up to n using the recurrence.     */
    for (idx_t i = length; i <= n; ++i) {
        cache[(size_t)i] = cache[(size_t)(i-1)]
                         + (seq_t)2 * cache[(size_t)(i-2)]
                         - cache[(size_t)(i-3)];
    }

    /* Update the record of filled terms.                            */
    if ((size_t)n + 1 > length) length = (size_t)n + 1;

    return cache[(size_t)n];
}
