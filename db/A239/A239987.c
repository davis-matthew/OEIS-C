/*
 * A239987  Number of 3 X n 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or two plus the sum of the elements
 *          above it or one plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A239987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 6 n^3 + 71 n^2 - 258 n + 552) / 24 (for n >= 4)
 *            Initial values: a(1) = 3, a(2) = 6, a(3) = 13
 * Range:     u8 n <= 8, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)13;

    /* Use 64‑bit arithmetic for the intermediate calculation.  The
       maximum value of n in the test set is far below 2^9, so n^4
       comfortably fits in uint64_t. */
    uint64_t N = (uint64_t)n;

    uint64_t N2 = N * N;        // n^2
    uint64_t N3 = N2 * N;       // n^3
    uint64_t N4 = N3 * N;       // n^4

    uint64_t num = N4
                 - 6 * N3
                 + 71 * N2
                 - 258 * N
                 + 552;
    return (seq_t)(num / 24);
}
