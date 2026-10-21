/*
 * A230340  Denominator of Sum_{k=1..n} 1/(k(k+1)(k+2)(k+3)) = Sum_{k=1..n}
 *          1/Pochhammer(k,4).
 * https://oeis.org/A230340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of 1/18 - 1/(3*(n+1)*(n+2)*(n+3)) = (18 * P)
 *            / gcd(P-6, 18*P) where P = (n+1)(n+2)(n+3)
 * Range:     u8 n <= 0, u16 n <= 13, u32 n <= 648, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for safe arithmetic */
    seq_t ns = (seq_t)n;
    /* Compute P = (n+1)(n+2)(n+3) */
    seq_t P = (ns + (seq_t)1) * (ns + (seq_t)2) * (ns + (seq_t)3);
    /* Numerator part of the fraction */
    seq_t num = P - (seq_t)6;
    /* Compute gcd(P-6, 18*P) */
    seq_t g = gcd(num, ((seq_t)18) * P);
    /* Denominator of the reduced fraction */
    return ((seq_t)18 * P) / g;
}
