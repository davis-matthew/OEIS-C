/*
 * A016164  Expansion of 1/((1-5*x)*(1-10*x)).
 * https://oeis.org/A016164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*10^n - 5^n, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 967 (2026-11-01)
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
    /* compute 10^n and 5^n */
    seq_t p10 = pow10(n);
    seq_t p5  = power5(n);

    /* a(n) = 2*10^n - 5^n */
    seq_t res = p10 * 2;
    res -= p5;
    return res;
}
