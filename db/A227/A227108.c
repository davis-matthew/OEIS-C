/*
 * A227108  Denominators of harmonic mean H(n,5), n >= 0.
 * https://oeis.org/A227108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = (n+5)/gcd(n+5,50) (denominator of 10*n/(n+5))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t and add the constant offset 5 */
    seq_t d = (seq_t)n + 5;

    /* Compute gcd with 50 – the gcd is always a divisor of 50 */
    seq_t g = gcd(d, (seq_t)50);

    /* Return the denominator */
    return d / g;
}
