/*
 * A010737  a(n) = 2*a(n-2) + 1.
 * https://oeis.org/A010737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 2 == 0) ? 5·2^(n/2) – 1 : 7·2^(n/2) – 1
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n / 2);
    if ((n & 1) == 0) {
        return ((seq_t)5 * p) - 1;
    } else {
        return ((seq_t)7 * p) - 1;
    }
}
