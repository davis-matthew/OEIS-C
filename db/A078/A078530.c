/*
 * A078530  Bilinear recursive sequence.
 * https://oeis.org/A078530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n mod 12) * (-1)^(⌊n/12⌋) * (-27)^{⌊n/12⌋ * (6⌊n/12⌋ +
 *            n mod 12 - 4)} with a(0)=0, a(1)=3, a(2)=1, a(3)=1, a(4)=1,
 *            a(5)=1, a(6)=2, a(7)=3, a(8)=9, a(9)=27, a(10)=81, a(11)=729
 * Range:     i8 n <= 10, i16 n <= 12, i32 n <= 16, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* initialise the first 12 values */
    static const seq_t init[12] = {
        (seq_t)0,  (seq_t)3,  (seq_t)1,  (seq_t)1,
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)3,
        (seq_t)9,  (seq_t)27, (seq_t)81, (seq_t)729
    };

    /* reduce n modulo 12 and compute quotient */
    idx_t r = n % 12;   /* r ∈ [0,11] */
    if (init[r] == (seq_t)0)   /* base case a(0)=0 or multiple of 12 */
        return init[r];

    idx_t q = n / 12;   /* q = floor(n/12) */

    /* exponent of 27 in the closed form */
    idx_t exp = q * (6 * q + r - 4);

    /* compute 27^exp, ignoring the sign of the base first */
    seq_t pow_pos = pow_int((seq_t)27, exp);

    /* sign contributed by the negative base (-27)^exp */
    seq_t sign_base = (exp & 1) ? (seq_t)-1 : (seq_t)1;

    /* combine everything */
    seq_t res = init[r] * pow_pos * sign_base;

    /* additional sign factor (-1)^q from the recurrence */
    if ((q & 1) != 0)
        res = -res;

    return res;
}
