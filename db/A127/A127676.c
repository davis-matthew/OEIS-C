/*
 * A127676  Numerators of partial sums of a series for Pi*sqrt(2)/4.
 * https://oeis.org/A127676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Sequence A127676
    Numerators of partial sums of a series for Pi*sqrt(2)/4
    a(n) = numerator of S(n) where
        S(n) = Σ_{k=0..n} (-1)^⌊k/2⌋ /(2k+1)
    The resulting rational is reduced to lowest terms.
*/

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base case: first term is 1/1 */
    if (n == 0U) {
        return (seq_t)1;
    }

    /* Current reduced fraction r = num/den
       with sign s (±1).  Initial value is 1/1. */
    seq_t num = (seq_t)1;
    seq_t den = (seq_t)1;
    int    s   = +1;   /* sign of the current sum */

    for (idx_t k = 1U; k <= n; ++k) {
        seq_t factor = (seq_t)2 * k + (seq_t)1;          /* 2*k+1   */
        int   t;                                      /* sign of term   */
        /* floor(k/2) is k>>1.  Two positives followed by two negatives. */
        t = (((k >> 1U) & 1U) == 0U) ? +1 : -1;

        /* Reduce the common part before multiplying to keep numbers small. */
        seq_t g1 = gcd(den, factor);
        seq_t den1   = den / g1;
        seq_t factor1 = factor / g1;

        /* Multiply numerator by the simplified factor. */
        seq_t mult1 = num * factor1;   /* always non‑negative */
        seq_t mult2 = den1;            /* non‑negative */

        seq_t new_num_part; /* part that will be added/subtracted */
        int   new_sign;

        if (s == t) {
            /* Same sign → add */
            new_sign    = s;
            new_num_part = mult1 + mult2;
        } else {
            /* Opposite signs → subtract the smaller from the larger */
            if (mult1 >= mult2) {
                new_sign      = s;
                new_num_part  = mult1 - mult2;
            } else {
                new_sign      = t;
                new_num_part  = mult2 - mult1;
            }
        }

        /* New denominator is lcm(den, factor) = den1 * factor1 * g1. */
        seq_t new_den = den1 * factor1 * g1;

        /* Reduce the new fraction to lowest terms */
        seq_t g2 = gcd(new_num_part, new_den);
        num = new_num_part / g2;
        den = new_den      / g2;
        s   = new_sign;   /* keep the sign for the next step (unused for return) */
    }

    /* All terms of the sequence are positive, so return the numerator. */
    return num;
}
