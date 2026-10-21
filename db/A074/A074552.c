/*
 * A074552  a(n) = 3^n + 5^n + 7^n.
 * https://oeis.org/A074552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n + 5^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for any seq_t, using binary exponentiation. */
static seq_t pow_seq(idx_t exp, seq_t base) {
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & (idx_t)1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p3 = pow_seq(n, (seq_t)3);
    seq_t p5 = pow_seq(n, (seq_t)5);
    seq_t p7 = pow_seq(n, (seq_t)7);
    return p3 + p5 + p7;
}
