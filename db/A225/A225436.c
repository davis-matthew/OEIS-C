/*
 * A225436  Denominators of convergents to the general continued fraction 1/(1
 *          + 2/(1 + 3/(1 + 4/(1+ ...)))).
 * https://oeis.org/A225436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, changed
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 58,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominator a(n) of the nth convergent of
   C = 1/(1 + 2/(1 + 3/(1 + 4/(1 + …)))).
   Let S_{k} be the value of
          1 + k/(1 + (k+1)/(1 + …))  (with S_{n+1} = 1).
   Recurrence (working backwards from the bottom):
        S_{k} = (p + k*q) / p          where S_{k+1} = p/q
        reduce (p + k*q, p) by gcd.
   After the loop S_2 = p/q;  C = 1 / (p/q) = q/p, so the required
   denominator is p.  This algorithm runs in O(n) time with
   arbitrary‑precision arithmetic when required. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* S_{n+1} = 1/1  --> p = numerator, q = denominator */
    seq_t p = 1, q = 1;

    for (idx_t t = n; t >= 2; --t)
    {
        /* new numerator = old numerator + t * old denominator */
        seq_t new_num = p + (seq_t)t * q;
        seq_t new_den  = p;

        /* reduce by gcd to keep numbers minimal */
        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        p = new_num;
        q = new_den;
    }

    /* final value C = q/p  → denominator of reduced fraction = p */
    return p;
}
