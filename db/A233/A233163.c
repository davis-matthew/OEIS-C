/*
 * A233163  Number of n X 3 0..7 arrays with no element x(i,j) adjacent to
 *          itself or value 7-x(i,j) horizontally, diagonally, antidiagonally
 *          or vertically, top left element zero, and 1 appearing before 2 3 4
 *          5 and 6, 2 appearing before 3 4 and 5, and 3 appearing before 4 in
 *          row major order (unlabelled 8-colo...
 * https://oeis.org/A233163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3, for n>1 : a(n) = 8^(n-2) * (2^n + 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    seq_t pow8 = big_pow((seq_t)8, (seq_t)(n - 2));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);
    return pow8 * (pow2 + (seq_t)4);
}
