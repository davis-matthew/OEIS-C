/*
 * A227107  Numerators of harmonic mean H(n,4), n >= 0.
 * https://oeis.org/A227107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 8*n/gcd(n+4,32)
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n_seq = n;
    seq_t numerator = 8 * n_seq;
    seq_t g = gcd(n_seq + 4, (seq_t)32);
    return numerator / g;
}
