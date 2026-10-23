/*
 * A137789  a(n) = 7^n - 6^n - 5^n - 4^n - 3^n - 2^n.
 * https://oeis.org/A137789
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 6^n - 5^n - 4^n - 3^n - 2^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp using repeated multiplication.
   Works for any seq_t type (uint8_t, uint16_t, ..., uint128_t,
   or arbitrary‑precision class). */
static inline seq_t _pow_u64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        result *= base;
        exp--;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = _pow_u64((seq_t)2, n);
    seq_t p3 = _pow_u64((seq_t)3, n);
    seq_t p4 = _pow_u64((seq_t)4, n);
    seq_t p5 = _pow_u64((seq_t)5, n);
    seq_t p6 = _pow_u64((seq_t)6, n);
    seq_t p7 = _pow_u64((seq_t)7, n);

    seq_t result = p7;
    result -= p6;
    result -= p5;
    result -= p4;
    result -= p3;
    result -= p2;
    return result;
}
