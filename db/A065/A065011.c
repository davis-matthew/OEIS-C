/*
 * A065011  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 8.
 * https://oeis.org/A065011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth positive integer m (m not a perfect square) for
 *            which * the first partial quotient after the integer part in the
 *            continued fraction * expansion of √m equals 8. * *
 *            Implementation: iteratively test successive numbers, applying
 *            the standard * continued‑fraction algorithm for √m until...
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, abort */
#include "../utils/floor_sqrt.h"  /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* static storage for the already computed members of the sequence      */
    static seq_t *cache   = NULL;   /* array of computed a(i) values          */
    static size_t  len    = 0;      /* number of terms currently cached       */
    static size_t  cap    = 0;      /* current capacity of the cache array    */
    static idx_t   cand   = 1;      /* next candidate number to examine      */

    /* Strip off the built‑in offset: we provide a(1) first.               */
    if (n == 0) return 0;   /* behaviour for the offset, not used by tests */

    while (len < n) {
        seq_t m, a0, d, a1;

        /* Skip perfect squares immediately.                               */
        m  = (seq_t)cand;
        a0 = floor_sqrt(m);
        if ((a0 * a0) == m) {
            cand++;
            continue;
        }

        /* Compute the first periodic part term (a1).                       */
        d  = m - a0 * a0;                 /* d1 = n - a0²                          */
        a1 = (a0 + a0) / d;                /* (a0 + m)/d where m = a0                */

        if (a1 == 8) {
            /* Cache the discovered term.                                  */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newptr) abort();
                cache = newptr;
                cap   = newcap;
            }
            cache[len++] = m;
        }

        cand++;
    }

    return cache[n - 1];
}
