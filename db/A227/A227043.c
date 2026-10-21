/*
 * A227043  Numerator of harmonic mean H(n,2), n>= 0.
 * https://oeis.org/A227043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(4*n/(n+2)) = 4*n/gcd(n+2,8)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)4 * (seq_t)n;          /* 4*n */
    seq_t den = (seq_t)n + 2;                /* n+2  */
    seq_t g   = gcd(den, (seq_t)8);          /* gcd(n+2,8) */
    return num / g;                          /* numerator of harmonic mean */
}
