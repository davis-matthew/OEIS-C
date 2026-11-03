/*
 * A036257  Number of inequivalent strings of 2n digits, when 2 strings are
 *          equivalent if turning 1 upside down gives the other.
 * https://oeis.org/A036257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, changed
 * Formula:   a(n) = 10^(2*n) - 5^(2*n)/2 + 5^n/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 396 (2026-11-01)
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
    /* compute the three powers */
    idx_t exp2n = 2 * n;
    seq_t ten_pow   = pow10(exp2n);   /* 10^(2n) */
    seq_t five_pow2n = power5(exp2n); /* 5^(2n) */
    seq_t five_pow_n = power5(n);     /* 5^n */

    /* use integer arithmetic only */
    seq_t two_ten_pow = ten_pow * 2;          /* 2 * 10^(2n) */
    seq_t diff        = two_ten_pow - five_pow2n; /* 2*10^(2n) - 5^(2n) */
    seq_t sum         = diff + five_pow_n;    /* 2*10^(2n) - 5^(2n) + 5^n */
    return sum / 2;                            /* divide by 2 (exact) */
}
