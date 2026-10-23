/*
 * A168059  Denominator of (n+2)/(n*(n+1)).
 * https://oeis.org/A168059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = n*(n+1) / gcd(n+2, n*(n+1))
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the denominator of the reduced fraction (n+2)/(n*(n+1)). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute n*(n+1) using seq_t to avoid overflow in intermediate types */
    seq_t num = (seq_t)n * ((seq_t)n + 1);
    /* gcd between the numerator of the fraction and the denominator */
    seq_t g   = gcd(num, (seq_t)(n + 2));
    return num / g;
}
