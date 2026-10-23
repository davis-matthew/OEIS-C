/*
 * A137480  a(n)=4a(n-2).
 * https://oeis.org/A137480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2), a(0)=12, a(1)=21
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    if ((n & 1ULL) == 0) {     /* n even */
        return (seq_t)12 * pow2(2 * k);
    } else {                   /* n odd */
        return (seq_t)21 * pow2(2 * k);
    }
}
