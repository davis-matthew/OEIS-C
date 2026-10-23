/*
 * A132824  Row sums of triangle A132823.
 * https://oeis.org/A132824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 2 + 2^n - 2*n for n > 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t val = pow2(n);                 /* 2^n */
    val -= (seq_t)2 * (seq_t)n;          /* subtract 2*n */
    val += (seq_t)2;                     /* add 2 */
    return val;
}
