/*
 * A227106  Numerators of harmonic mean H(n,3), n >= 0.
 * https://oeis.org/A227106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6*n/gcd(n+3,6*n)
 * Range:     u8 n <= 42, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t num   = n_seq * (seq_t)6;
    seq_t den   = n_seq + (seq_t)3;
    seq_t g     = gcd(den, num);
    return num / g;
}
