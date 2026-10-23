/*
 * A158730  a(n) = 68*n^2 - 1.
 * https://oeis.org/A158730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*n^2 - 1
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 7947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return (N * N) * (seq_t)68 - (seq_t)1;
}
