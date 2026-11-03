/*
 * A201058  Numerator of binomial(2n,n)/(2n).
 * https://oeis.org/A201058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of binomial(2n,n)/(2n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* C(2n, n) */
    seq_t num = binomial(2 * n, n);
    /* Denominator 2n */
    seq_t denom = ((seq_t)n) * 2;
    /* Reduce fraction */
    seq_t g = gcd(num, denom);
    return num / g;
}
