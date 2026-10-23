/*
 * A017018  a(n) = (7*n + 3)^2.
 * https://oeis.org/A017018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 3)^2
 * Range:     u8 n <= 1, u16 n <= 36, u32 n <= 9361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 7 + 3;
    return m * m;
}
