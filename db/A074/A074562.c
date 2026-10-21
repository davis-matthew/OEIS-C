/*
 * A074562  a(n) = 4^n + 5^n + 7^n.
 * https://oeis.org/A074562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring for integer exponents.
   Works for all supported seq_t types, including arbitrary-precision. */
static inline seq_t pow_uint(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow4 = pow_uint((seq_t)4, n);
    seq_t pow5 = pow_uint((seq_t)5, n);
    seq_t pow7 = pow_uint((seq_t)7, n);
    return pow4 + pow5 + pow7;
}
