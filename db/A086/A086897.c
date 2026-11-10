/*
 * A086897  a(n) =
 *          1/5!*(A000522(5*n)+10*A000522(3*n)+20*A000522(2*n)+45*A000522(n)+44).
 * https://oeis.org/A086897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000522(5*n) + 10*A000522(3*n) + 20*A000522(2*n) +
 *            45*A000522(n) + 44)/120, where A000522(m) = m*A000522(m-1) + 1,
 *            A000522(0) = 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 48 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000522_a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result = (seq_t)i * result + 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t s = A000522_a(5 * n)
            + (seq_t)10 * A000522_a(3 * n)
            + (seq_t)20 * A000522_a(2 * n)
            + (seq_t)45 * A000522_a(n)
            + (seq_t)44;
    return s / 120;
}
