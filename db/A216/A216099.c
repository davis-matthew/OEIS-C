/*
 * A216099  Period of powers of 3 mod 10^n.
 * https://oeis.org/A216099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*5^(n-1) for n ≤ 4, * a(n) = 5*10^(n-2) for n ≥ 5 * (and
 *            equivalently a(n) = 10*a(n-1) for n > 4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n <= 4) {
        seq_t v = power5(n - 1);          /* 5^(n-1)  */
        return seq_mul(v, (seq_t)4);      /* 4*5^(n-1) */
    } else {
        seq_t v = pow10(n - 2);           /* 10^(n-2) */
        return seq_mul(v, (seq_t)5);      /* 5*10^(n-2) */
    }
}
