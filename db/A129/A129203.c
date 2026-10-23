/*
 * A129203  a(n) = numerator(3/(n+1)^3)*(3/2 + (-1)^n/2).
 * https://oeis.org/A129203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 / gcd(n+1, 6)
 * Range:     u8 n <= 488, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type. */
    seq_t val = (seq_t)(n + 1);   /* n+1 is guaranteed to fit in seq_t */
    seq_t six = (seq_t)6;

    /* Compute the greatest common divisor. */
    seq_t g = gcd(val, six);

    /* Return the required quotient. */
    return six / g;
}
