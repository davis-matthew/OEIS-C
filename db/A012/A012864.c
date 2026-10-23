/*
 * A012864  Take every 5th term of Padovan sequence A000931, beginning with
 *          the first term.
 * https://oeis.org/A012864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=3,
   a(n) = 5*a(n-1) - 4*a(n-2) + a(n-3)  for n ≥ 3 */
#include <stdlib.h>

/* Computes a(n) for the OEIS sequence A012864 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoisation array          */
    static size_t cap   = 0;          /* array capacity (bytes)    */
    static size_t last  = (size_t)(-1);/* highest index currently computed */

    /* Ensure array has room for indices up to n */
    if (n + 1 > cap)
    {
        size_t new_cap = (size_t)(n + 1);
        /* Allocate at least 3 entries for the initial values */
        if (new_cap < 3) new_cap = 3;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure   */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first terms if this is the very first use      */
    if (last == (size_t)(-1))
        last = 0;

    /* Compute all missing terms up to n */
    for (size_t i = last; i <= (size_t)n; ++i)
    {
        if (i == 0)           cache[0] = 1;
        else if (i == 1)      cache[1] = 1;
        else if (i == 2)      cache[2] = 3;
        else                  cache[i] = 5 * cache[i-1] - 4 * cache[i-2] + cache[i-3];
    }

    last = (size_t)n;
    return cache[n];
}
