/*
 * A026573  a(n)=T(n,n-4), T given by A026568. Also a(n) = number of integer
 *          strings s(0),...,s(n) counted by T, such that s(n)=4.
 * https://oeis.org/A026573
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A026573
   Formula: a(n) = T(n,n-4), where T satisfies
      -2*(n-4)*(n+4)*a(n) +(5*n^2-5*n-46)*a(n-1) +(4*n^2+n+18)*a(n-2)
      +(-11*n^2+24*n-16)*a(n-3) +2*(n-2)*(2*n-5)*a(n-4)=0 , for n ≥ 8
   Initial values: a(4)=1, a(5)=3, a(6)=12, a(7)=34.
   The recurrence produces integer results for all n ≥ 4.
   The implementation memoises previously computed terms and is
   thread‑safe when used from a single thread.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;     /* undefined for indices below the offset */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;
    static size_t  next   = 8;      /* next index that still needs to be computed; 8 is
                                      * the first index computed by recurrence, 4..7
                                      * are initialised explicitly */

    /* resize cache if necessary */
    if (n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 8;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { exit(EXIT_FAILURE); }
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first four values (a(4)..a(7)) */
    cache[4] = (seq_t)1;
    cache[5] = (seq_t)3;
    cache[6] = (seq_t)12;
    cache[7] = (seq_t)34;

    /* compute missing entries up to n */
    for (idx_t i = next; i <= n; ++i) {
        int64_t ii = (int64_t)i;          /* 64‑bit signed for coefficient arithmetic */

        int64_t c1 = 5*ii*ii - 5*ii - 46;                 /* positive or zero */
        int64_t c2 = 4*ii*ii + ii + 18;                  /* positive */
        int64_t c3 = -11*ii*ii + 24*ii - 16;             /* negative for i≥4 */
        int64_t c4 = 2*((ii-2)*(2*ii-5));                /* positive */
        int64_t denom = 2*((ii-4)*(ii+4));               /* positive for i≥8 */

        seq_t term1 = (seq_t)c1 * cache[i-1];
        seq_t term2 = (seq_t)c2 * cache[i-2];
        /* c3 is negative → subtract the positive magnitude                       */
        seq_t term3 = (seq_t)(-c3) * cache[i-3];
        seq_t term4 = (seq_t)c4 * cache[i-4];

        seq_t num = term1 + term2 - term3 + term4;
        cache[i] = num / (seq_t)denom;        /* exact division */
    }
    next = (size_t)n + 1;                     /* all indices up to n are now computed */

    return cache[n];
}
