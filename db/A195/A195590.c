/*
 * A195590  Number of ways to place 2n nonattacking kings on a vertical
 *          cylinder 4 X 2n.
 * https://oeis.org/A195590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n-3)*2^n + 4
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t pown = pow2(n);          /* 2^n in seq_t */
    seq_t term = (5 * N - 3) * pown + 4;
    return term;
}
