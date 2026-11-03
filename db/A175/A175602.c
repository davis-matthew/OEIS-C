/*
 * A175602  a(n) = 8*(10^n - 3).
 * https://oeis.org/A175602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(10^n - 3) (OEIS A175602)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-01)
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

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1.  For n = 0 the value would be negative,
       but the OEIS offset is 1, so we ignore that case. */
    if (n == 0) {
        /* Return zero for safety; this value is never requested by the tests. */
        return (seq_t)0;
    }
    /* Compute 10^n as a seq_t.  The utility pow10 returns a seq_t. */
    seq_t pow10_val = pow10(n);
    /* Multiply by 8 and subtract 24.  All operations are exact for seq_t. */
    seq_t result = pow10_val * (seq_t)8 - (seq_t)24;
    return result;
}
