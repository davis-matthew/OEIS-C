/*
 * A161712  a(n) = (4*n^3 - 6*n^2 + 8*n + 3)/3.
 * https://oeis.org/A161712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 6*n^2 + 8*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1175, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = 4 * n3 - 6 * n2 + 8 * nn + 3;
    return num / 3;
}
