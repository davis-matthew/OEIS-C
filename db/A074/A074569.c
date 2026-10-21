/*
 * A074569  a(n) = 4^n + 7^n + 9^n.
 * https://oeis.org/A074569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for arbitrary-precision or fixed-width unsigned types. */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A074569 */
seq_t a(idx_t n) {
    seq_t p4 = pow_seq(4, n);
    seq_t p7 = pow_seq(7, n);
    seq_t p9 = pow_seq(9, n);
    return p4 + p7 + p9;
}
