/*
 * A030975  [ exp(1/4)*n! ].
 * https://oeis.org/A030975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(exp(1/4) * n!)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);        /* n!  */
    seq_t carry = 0;                  /* integer part of the sum           */
    seq_t rem   = 0;                  /* remainder when expressed over 4^k  */
    seq_t rem_den = 1;                /* current denominator 4^k            */
    seq_t numer = fact;               /* Q_k = n! / k!   */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term_int = numer / rem_den;   /* integer part of the term   */
        seq_t term_rem = numer % rem_den;   /* fractional remainder       */
        carry += term_int;                  /* add integer part           */
        rem   += term_rem;                  /* add fractional part        */

        /* advance to next k */
        numer   = numer / (seq_t)(k + 1);    /* Q_{k+1} = Q_k / (k+1)      */
        rem_den = rem_den * 4;              /* 4^(k+1)                    */
        rem     = rem * 4;                  /* adjust remainder to new denom */

        /* carry over any whole 1s from the remainder               */
        if (rem >= rem_den)
        {
            seq_t add = rem / rem_den;
            carry += add;
            rem = rem % rem_den;
        }
    }

    return carry;
}
