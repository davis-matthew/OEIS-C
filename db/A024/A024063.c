/*
 * A024063  a(n) = 6^n - n.
 * https://oeis.org/A024063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute 6^n by binary exponentiation */
static seq_t pow6(idx_t n) {
    seq_t result = 1;
    seq_t base = 6;
    while (n) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t power = pow6(n);
    return power - (seq_t)n;
}
