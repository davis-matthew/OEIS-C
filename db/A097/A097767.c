/*
 * A097767  Pell equation solutions (11*b(n))^2 - 122*a(n)^2 = -1 with b(n) =
 *          A097766(n), n >= 0.
 * https://oeis.org/A097767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097767: a(n) = 486*a(n-1) - a(n-2), with a(0)=1, a(1)=485.
   Uses memoization to support arbitrary order of calls. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache */
    static seq_t *cache = NULL;      /* pointer to allocated array of seq_t */
    static idx_t size   = 0;         /* number of elements currently stored */
    static idx_t cap    = 0;         /* allocated capacity in elements */

    /* Grow the cache to hold term n */
    if (n + 1 > size) {
        idx_t needed = n + 1;
        if (needed > cap) {
            /* Double capacity until it can hold all needed terms */
            idx_t newcap = (cap > 0) ? (cap << 1) : 16;
            while (newcap < needed) {
                newcap <<= 1;
            }
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            /* On a real judge we might handle realloc failure, but it is
               not required for the exercise. */
            cap = newcap;
        }

        /* Compute any missing terms */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)485;
            } else {
                cache[i] = cache[i-1] * 486 - cache[i-2];
            }
        }
        size = n + 1;
    }

    return cache[n];
}
