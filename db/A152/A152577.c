/*
 * A152577  a(n) = 10^(2*n - 1) + 1.
 * https://oeis.org/A152577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(2*n - 1) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* n must be >= 1 (OEIS offset is 1) */
    idx_t exp = n;    /* start with n */
    exp += n;         /* exp = 2*n */
    exp -= 1;         /* exp = 2*n - 1 */
    seq_t value = pow10(exp);   /* compute 10^(2*n-1) */
    value += 1;                  /* add the trailing 1 */
    return value;
}
