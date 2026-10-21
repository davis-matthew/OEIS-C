/*
 * A227109  Numerators of harmonic mean H(n, 5), n >= 0.
 * https://oeis.org/A227109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 10*n/gcd(n+5,10*n)
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* For n = 0 the formula naturally yields 0. */
    seq_t ns = (seq_t)n;
    seq_t num = ns * (seq_t)10;
    seq_t den = ns + (seq_t)5;
    seq_t g   = gcd(den, num);
    return num / g;
}
