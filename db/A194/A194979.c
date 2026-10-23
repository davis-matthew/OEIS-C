/*
 * A194979  a(n) = 1 + floor(n/sqrt(3)).
 * https://oeis.org/A194979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n / sqrt(3))
 * Range:     u8 n <= 13, u16 n <= 632, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;          /* n^2 */
    seq_t div = n2 / (seq_t)3;  /* floor(n^2 / 3) */
    seq_t sq  = floor_sqrt(div);/* floor(sqrt(div)) == floor(n / sqrt(3)) */
    return sq + 1;
}
