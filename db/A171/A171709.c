/*
 * A171709  Numerator of 1/9 - 1/n^2.
 * https://oeis.org/A171709
 *
 * Offset:    -3    (a(-3) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = numerator of (1/9 - 1/n^2)
 * Range:     i8 n <= 3, i16 n <= 122, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For all n >= -3.  a(0) is defined as -1 at the pole.  */
/* Uses the provided gcd helper from "../utils/gcd.h". */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Special case of the pole at n = 0 */
    if (n == 0)
        return (seq_t)(-1);

    /* Work with |n| because a(n) = a(-n) */
    idx_t abs_n = (n < 0) ? -n : n;

    /* Compute n^2, numerator = n^2 - 9, denominator = 9 n^2 */
    seq_t nn      = (seq_t)abs_n * (seq_t)abs_n;
    seq_t num     = nn - (seq_t)9;          /* can be negative */
    seq_t denom   = (seq_t)9 * nn;          /* always positive */

    /* Reduce the fraction (n^2 - 9) / (9 n^2) */
    seq_t num_abs = (num < 0) ? -num : num; /* absolute numerator */
    seq_t g       = gcd(num_abs, denom);    /* gcd for seq_t */

    seq_t result = num_abs / g;
    if (num < 0)
        result = -result;                    /* restore the sign */

    return result;
}
