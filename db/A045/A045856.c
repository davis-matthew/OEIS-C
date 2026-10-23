/*
 * A045856  Numbers whose square has initial digit '2'.
 * https://oeis.org/A045856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate integers, keep those whose square starts with digit 2
   (OEIS A045856).  The function maintains a static cache so that calls
   with arbitrary n are answered in O(n) time. */

#include <stdint.h>
#include <stdlib.h>                 /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* static cache of found terms */
    static seq_t *cache   = NULL;   /* array of seq_t */
    static size_t cap     = 0;      /* allocated capacity */
    static size_t len     = 0;      /* number of cached terms */
    static idx_t  cur     = 1;      /* next integer to test */

    /* initialise the cache on first call */
    if (!cache) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* extend cache until it contains at least n terms */
    while (len < (size_t)n) {
        seq_t m   = cur++;            /* candidate integer */
        seq_t sq  = m * m;            /* its square */
        seq_t d   = sq;               /* strip trailing digits */

        /* find the most significant decimal digit of sq */
        while (d >= 10) d /= 10;

        /* keep the candidate if the leading digit is 2 */
        if (d == 2) {
            if (len == cap) {         /* grow the cache if full */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = m;
        }
    }

    /* return the requested term (a(1) at index 0) */
    return cache[n - 1];
}
