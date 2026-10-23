/*
 * A168430  a(n) = 4^n mod 13.
 * https://oeis.org/A168430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n mod 13
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 1;
    seq_t base = 4;
    for (idx_t i = 0; i < n; ++i) {
        res = (res * base) % (seq_t)13;
    }
    return res;
}
