/*
 * A165222  a(n) = (2^(n+4)+1)*(2^n+1).
 * https://oeis.org/A165222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+4)+1)*(2^n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t t1 = pow2(n + 4);   /* 2^(n+4) */
    t1 += (seq_t)1;           /* +1 */
    seq_t t2 = pow2(n);       /* 2^n  */
    t2 += (seq_t)1;           /* +1 */
    return t1 * t2;           /* product */
}
