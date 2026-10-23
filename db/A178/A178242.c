/*
 * A178242  Numerator of n*(5+n)/((n+1)*(n+4)).
 * https://oeis.org/A178242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of n*(n+5)/((n+1)*(n+4))
 * Range:     u8 n <= 13, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t num = (seq_t)n * ((seq_t)n + (seq_t)5);
    seq_t den = ((seq_t)n + (seq_t)1) * ((seq_t)n + (seq_t)4);
    seq_t g   = gcd(num, den);
    return num / g;
}
