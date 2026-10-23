/*
 * A155646  a(n) = 8^n + 6^n - 1.
 * https://oeis.org/A155646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 6^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) {
            result = result * base;
        }
        exp >>= 1;
        if (exp) {
            base = base * base;
        }
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p8 = pow_seq((seq_t)8, n);
    seq_t p6 = pow_seq((seq_t)6, n);
    return p8 + p6 - (seq_t)1;
}
