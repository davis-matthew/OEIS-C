/*
 * A138826  Concatenation of 2n-1 digits 1, n digits 0 and 2n-1 digits 1.
 * https://oeis.org/A138826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (10^(2*n-1) - 1 + 10^(5*n-2) - 10^(3*n-1))/9
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 174 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the concatenation of (2*n-1) 1's, followed by n 0's, followed by (2*n-1) 1's. */

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

seq_t a(idx_t n)
{
    /* Exponent calculations – use idx_t arithmetic safely for the expected bounds. */
    idx_t e2 = n + n - 1;          /* 2*n - 1 */
    idx_t e3 = n * 3 - 1;          /* 3*n - 1 */
    idx_t e5 = n * 5 - 2;          /* 5*n - 2 */

    /* Pre‑compute powers of ten. */
    seq_t pow_2n1 = pow10(e2);     /* 10^(2*n-1) */
    seq_t pow_3n1 = pow10(e3);     /* 10^(3*n-1) */
    seq_t pow_5n2 = pow10(e5);     /* 10^(5*n-2) */

    /* Apply the closed form: (pow_2n1 - 1 + pow_5n2 - pow_3n1) / 9. */
    seq_t numerator = pow_2n1 - 1 + pow_5n2 - pow_3n1;
    return numerator / 9;
}
