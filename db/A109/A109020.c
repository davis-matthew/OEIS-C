/*
 * A109020  a(n) = (2*7^n - 3*3^n + 1)/6.
 * https://oeis.org/A109020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*7^n - 3*3^n + 1)/6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Utility: integer exponentiation by repeated squaring */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

extern seq_t a(idx_t n) {
    seq_t pow7 = pow_seq((seq_t)7, n);
    seq_t pow3 = pow_seq((seq_t)3, n);

    /* Compute 2*7^n + 1 - 3*3^n, avoiding unsigned underflow */
    seq_t num = (seq_t)2 * pow7 + (seq_t)1;
    num -= (seq_t)3 * pow3;

    return num / (seq_t)6;
}
