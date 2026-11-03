/*
 * A199320  a(n) = 5*6^n+1.
 * https://oeis.org/A199320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*6^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 969 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 6^n using exponentiation by squaring. */
static seq_t pow_seq6(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 6;
    while (n) {
        if (n & 1) result *= base;
        base *= base;
        n >>= 1;
    }
    return result;
}

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    seq_t pow6 = pow_seq6(n);
    return 5 * pow6 + 1;
}
