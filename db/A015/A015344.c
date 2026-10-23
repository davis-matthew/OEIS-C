/*
 * A015344  Gaussian binomial coefficient [ n,7 ] for q = -5.
 * https://oeis.org/A015344
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   Gaussian binomial coefficient [n,7] for q = -5
 * Range:     i8 none, i16 none, i32 n <= 7, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 7) {
        /* The sequence is defined for n >= 7; values below are arbitrary. */
        return 0;
    }

    /* Denominators: (-5)^i - 1 for i = 1..7 */
    static const seq_t denom_arr[7] = {
        (seq_t)(-6),          /* (-5)^1 - 1  = -6   */
        (seq_t)24,            /* (-5)^2 - 1  = 24   */
        (seq_t)(-126),        /* (-5)^3 - 1  = -126 */
        (seq_t)624,           /* (-5)^4 - 1  = 624  */
        (seq_t)(-3126),       /* (-5)^5 - 1  = -3126*/
        (seq_t)15624,         /* (-5)^6 - 1  = 15624*/
        (seq_t)(-78126)       /* (-5)^7 - 1  = -78126*/
    };

    /* Compute (-5)^n iteratively */
    seq_t current_power = 1;
    for (idx_t i = 0; i < n; ++i) {
        current_power *= (seq_t)(-5);
    }

    seq_t res = 1;

    for (idx_t i = 0; i < 7; ++i) {
        /* numerator = (-5)^(n+1-(i+1)) - 1 = current_power - 1 */
        seq_t numerator = current_power - 1;
        seq_t denom = denom_arr[i];

        /* Cancel factors of denom with current result */
        seq_t g1 = gcd(denom, res);
        denom /= g1;
        res /= g1;

        /* Cancel remaining factors of denom with numerator */
        seq_t g2 = gcd(numerator, denom);
        numerator /= g2;
        denom /= g2;

        /* Multiply and divide */
        res *= numerator;
        if (denom != 1) {
            res /= denom;   /* denom should be 1 after cancellations */
        }

        /* Prepare power for the next exponent (decrement by one) */
        current_power /= (seq_t)(-5);
    }

    return res;
}
