/*
 * A194069  a(n) = 1+floor((2/3)*n^2).
 * https://oeis.org/A194069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor((2/3)*n^2)
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t val = 1 + ((nn * nn) * 2) / 3;
    return val;
}
