/*
 * A116940  Greatest m such that A116939(m) = n.
 * https://oeis.org/A116940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + ceil(n^2/2) (OEIS A116940)
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;            /* convert index to seq_t */
    seq_t ceil_half_square = (N * N + (seq_t)1) / (seq_t)2;  /* ceil(n^2/2) */
    return (seq_t)2 * N + ceil_half_square;                  /* 2*n + ceil(n^2/2) */
}
