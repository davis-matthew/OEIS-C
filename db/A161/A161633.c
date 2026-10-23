/*
 * A161633  E.g.f. satisfies A(x) = 1/(1 - x*exp(x*A(x))).
 * https://oeis.org/A161633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..n} binomial(n+1,k)/(n+1) * k^{n-k}/(n-k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A161633. */

#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t falling = 1;          /* product n * (n-1) * … * (n-k+1) for current k   */
    seq_t binom   = 1;          /* binomial(n+1,k) for current k                  */
    seq_t total   = 0;          /* running sum of the terms                       */
    seq_t denom   = (seq_t)(n + 1);   /* common divisor (n+1)                          */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t powk;
        if (k == 0)
        {
            /* 0^0 = 1, 0^positive = 0 */
            powk = (n == 0) ? (seq_t)1 : (seq_t)0;
        }
        else
        {
            powk = big_pow((seq_t)k, (seq_t)(n - k));
        }

        /* term = falling * binom * powk / denom */
        seq_t term = falling * binom;
        term = term * powk;
        term = term / denom;     /* exact division */

        total += term;

        if (k == n) break;

        /* update falling factorial: multiply by (n-k) */
        falling *= (seq_t)(n - k);

        /* update binomial coefficient: binom(k+1) = binom(k) * (n+1-k)/(k+1) */
        binom *= (seq_t)((n + 1) - k);
        binom /= (seq_t)(k + 1);
    }

    return total;
}
