/*
 * A158003  a(n) = 196*n^2 - n.
 * https://oeis.org/A158003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 196*n^2 - n
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1-indexed; handle n == 0 defensively */
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    return 196 * sn * sn - sn;
}
