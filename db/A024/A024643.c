/*
 * A024643  n written in fractional base 7/6.
 * https://oeis.org/A024643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = representation of n in fractional base 7/6 (digits 0..6)
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 76, u64 n <= 433,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t m = (seq_t)n;
    /* The number of digits for n up to 2^64-1 is below 300. */
    uint8_t buf[300];
    size_t len = 0;
    while (m > 0) {
        seq_t d = m % (seq_t)7;
        buf[len++] = (uint8_t)d;
        m = (m - d) * (seq_t)6 / (seq_t)7;
    }
    seq_t res = 0;
    for (size_t i = 0; i < len; ++i) {
        res = res * (seq_t)10 + (seq_t)buf[len - 1 - i];
    }
    return res;
}
