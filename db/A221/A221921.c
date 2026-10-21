/*
 * A221921  a(n) = 4*n/gcd(4*n,n+4), n >= 4.
 * https://oeis.org/A221921
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n/gcd(16,n+4)
 * Range:     u8 n <= 63, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t four_n = nn * ((seq_t)4);
    seq_t divisor = gcd(((seq_t)16), ((seq_t)(n + 4)));
    return four_n / divisor;
}
