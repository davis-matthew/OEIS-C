/*
 * A137465  1 concatenated with n n's concatenated with 1.
 * https://oeis.org/A137465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 1 concatenated with n repeated n times concatenated with
 *            1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Compute the n-th term of OEIS sequence A137465 */
seq_t a(idx_t n)
{
    /* Compute number of decimal digits of n (n ≥ 1). */
    int len = 0;
    idx_t t = n;
    while (t > 0) {
        t /= 10;
        ++len;
    }

    /* pow10_len = 10^len  (typical 10, 100, 1000, etc.) */
    seq_t pow10_len = pow10((idx_t)len);

    seq_t res = 1;   /* leading 1 */

    /* Append n, n times */
    for (idx_t i = 0; i < n; ++i) {
        res = res * pow10_len + (seq_t)n;
    }

    /* Append trailing 1 */
    res = res * (seq_t)10 + (seq_t)1;

    return res;
}
