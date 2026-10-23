/*
 * A045654  Number of 2n-bead balanced binary strings, rotationally equivalent
 *          to complement.
 * https://oeis.org/A045654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (if n = 0); otherwise a(n) = Σ_{k=0..v₂(n)} 2^(n /
 *            2^k), where v₂(n) is the exponent of the largest power of 2
 *            dividing n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t m = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        m++;
    }

    seq_t sum = 0;
    for (idx_t k = 0; k <= m; ++k) {
        idx_t exp = n >> k;          /* n / 2^k */
        sum += pow2(exp);            /* 2^(exp) */
    }
    return sum;
}
