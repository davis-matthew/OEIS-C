/*
 * A081685  a(n) = 6^n - 5^n - 4^n - 3^n + 3*2^n.
 * https://oeis.org/A081685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6^n - 5^n - 4^n - 3^n + 3*2^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast binary exponentiation for arbitrary sized integer types */
/* base must be a small constant convertible to seq_t, exp is non‑negative */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p2 = pow_seq_t(2, n);
    seq_t p3 = pow_seq_t(3, n);
    seq_t p4 = pow_seq_t(4, n);
    seq_t p5 = pow_seq_t(5, n);
    seq_t p6 = pow_seq_t(6, n);
    return p6 - p5 - p4 - p3 + 3 * p2;
}
