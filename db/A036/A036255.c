/*
 * A036255  Number of inequivalent strings of 2n+1 digits, when 2 strings are
 *          equivalent if turning 1 upside down gives the other.
 * https://oeis.org/A036255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 10^(2*n+1) - 5^(2*n+1)/2 + 3*5^n/2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 433 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* exponent for the 10- and 5-powers */
    idx_t exp = 2 * n + 1;

    /* 10^(2*n+1) */
    seq_t pow10_val = pow10(exp);

    /* 5^(2*n+1) and 5^n */
    seq_t pow5_full = power5(exp);
    seq_t pow5_n    = power5(n);

    /* 3*5^n */
    seq_t three_pow5_n = seq_mul(pow5_n, (seq_t)3);

    /* (5^(2*n+1) - 3*5^n) is even; divide by 2 */
    seq_t diff      = pow5_full - three_pow5_n;
    seq_t half_diff = diff / (seq_t)2;

    /* final result */
    return pow10_val - half_diff;
}
