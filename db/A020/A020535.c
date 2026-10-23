/*
 * A020535  a(n) = 9th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2^(2*n) + 1) * (2^(6*n) + 6*2^(4*n) + 9*2^(2*n) + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t p2n = pow2(2 * n);
    seq_t p4n = pow2(4 * n);
    seq_t p6n = pow2(6 * n);
    return (p2n + 1) * (p6n + 6 * p4n + 9 * p2n + 1);
}
