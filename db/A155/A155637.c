/*
 * A155637  a(n) = 10^n+5^n-1.
 * https://oeis.org/A155637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n + 5^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 992 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t res = pow10(n);      /* 10^n  */
    res += power5(n);          /* + 5^n */
    res -= 1;                  /* - 1   */
    return res;
}
