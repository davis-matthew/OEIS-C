/*
 * A020536  a(n) = 10th Fibonacci polynomial evaluated at 2^n.
 * https://oeis.org/A020536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2^n * (2^(4*n) + 3*2^(2*n) + 1) * (2^(4*n) + 5*2^(2*n) +
 *            5)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t two_n  = pow2(n);           /* 2^n */
    seq_t two_2n = two_n * two_n;     /* 2^(2n) */
    seq_t two_4n = two_2n * two_2n;   /* 2^(4n) */

    seq_t term1 = two_4n + (seq_t)3 * two_2n + (seq_t)1;
    seq_t term2 = two_4n + (seq_t)5 * two_2n + (seq_t)5;

    return two_n * term1 * term2;
}
