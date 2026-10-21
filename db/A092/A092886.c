/*
 * A092886  Expansion of x/(x^4-x^3-2x^2-x+1).
 * https://oeis.org/A092886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=1, a(3)=3;
   a(n)=a(n-1)+2*a(n-2)+a(n-3)-a(n-4)  for n ≥ 4. */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Preallocate enough space for the problem limits (≈10 000). */
    static seq_t cache[20025] = {0};
    static idx_t  cached      = 0;          /* count of filled entries */

    /* Initialise the first four terms once. */
    if (cached == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cached   = 4;
    }

    /* Compute missing terms up to the requested index. */
    while (cached <= n) {
        seq_t term = cache[cached-1]            /* a(n-1) */
                   + cache[cached-2]            /* +a(n-2) */
                   + cache[cached-2]            /* +a(n-2) */
                   + cache[cached-3]            /* +a(n-3) */
                   - cache[cached-4];           /* -a(n-4) */
        cache[cached++] = term;
    }

    return cache[n];
}
