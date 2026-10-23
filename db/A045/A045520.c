/*
 * A045520  Numbers k such that k! has initial digit '1'.
 * https://oeis.org/A045520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computes the nth integer k (1‑based) such that the first
 *            decimal digit of k! is 1. The value is found by scanning k in
 *            increasing order and keeping the fractional part of log10(k!)
 *            (which is the leading digit information). Since the sequence
 *            starts with a(1)=0 we examine k=0 first
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* log10(2) is the boundary for leading digit 1 */
    static const long double LOG10_2 =
        0.3010299956639811952137388947244930267682L;  /* log10(2) */

    long double cum_log = 0.0L;   /* log10(k!) for current k */
    idx_t k = 0;                  /* current integer */
    idx_t count = 0;              /* how many terms found so far */

    while (1) {
        /* Leading digit of k! depends on fractional part of cum_log */
        long double frac = cum_log - floorl(cum_log);
        if (frac < LOG10_2) {
            count++;
            if (count == n)
                return (seq_t)k;  /* found the nth term */
        }

        /* advance to (k+1)! */
        k++;
        cum_log += log10l((long double)k);  /* cum_log = log10((k)!) */
    }
}
