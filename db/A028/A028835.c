/*
 * A028835  Numbers whose iterated sum of digits is a prime.
 * https://oeis.org/A028835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 9*floor((n-1)/4) + [2,3,5,7][(n-1) mod 4]
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    static const uint8_t residues[4] = {2, 3, 5, 7};
    idx_t pos   = n - 1;
    idx_t group = pos / 4;
    idx_t idx   = pos % 4;
    seq_t res   = (seq_t)residues[idx];
    return ((seq_t)group) * ((seq_t)9) + res;
}
