/*
 * A239138  The sequence S = a(1), a(2), ... is defined by a(1)=1, if d,e,f
 *          are consecutive digits then we do not have d <= e > f, and S is
 *          always extended with the smallest integer not yet present in S.
 * https://oeis.org/A239138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for 1 ≤ n ≤ 9, otherwise a(n) = 10^(n-8) – 1
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
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
    if (n <= 9)
        return (seq_t)n;

    seq_t val = pow10(n - 8);
    return val - 1;
}
