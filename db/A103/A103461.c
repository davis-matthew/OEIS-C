/*
 * A103461  a(n) = (-10)^n + 1 - 0^n.
 * https://oeis.org/A103461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-10)^n + 1 - 0^n * For n = 0, a(0) = 1 (since 0^0 = 1).
 *            For n > 0, 0^n = 0, so a(n) = (-10)^n + 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 841 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n) {
    /* Special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute 10^n */
    seq_t pow10_n = pow10(n);

    /* (-10)^n = (-1)^n * 10^n; use parity of n */
    if (n & 1U)    /* odd n */
        return (-pow10_n) + 1;
    else           /* even n */
        return pow10_n + 1;
}
