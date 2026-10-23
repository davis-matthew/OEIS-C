/*
 * A100312  Number of 3 X n binary matrices avoiding simultaneously the right
 *          angled numbered polyomino patterns (ranpp) (10;0) and (01;1).
 * https://oeis.org/A100312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0 a(n) = 2^(n-1) * (n^2 + 5*n + 2) for n > 0
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t poly = nn * nn + (seq_t)5 * nn + (seq_t)2;
    seq_t pow = pow2(n - 1);
    return pow * poly;
}
