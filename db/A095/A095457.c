/*
 * A095457  a(n) = 32 written in base 10 - n.
 * https://oeis.org/A095457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer equal to the digits of 32 in base (10 −
 *            n)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base = 10 − n */
    int64_t b = (int64_t)10 - (int64_t)n;

    /* Undefined (or unary) cases */
    if (b == 0 || b == -1) {           /* base 0 or −1 – no representation */
        return (seq_t)32;
    }
    if (b == 1) {                     /* unary representation */
        seq_t res = 0;
        for (int i = 0; i < 32; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Regular conversion of 32 to base b (b ≠ 0, ±1 ) */
    int64_t val = 32;
    int digits[64];
    int len = 0;
    int64_t abs_b = (b < 0) ? -b : b;

    while (val != 0) {
        int64_t r = val % b;
        if (r < 0) {
            r += abs_b;
        }
        digits[len++] = (int)r;
        val = (val - r) / b;
    }

    /* Assemble the decimal integer from the base‑b digits */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        int k = num_digits((seq_t)d);    /* number of decimal digits of d */
        res = res * pow10((idx_t)k) + (seq_t)d;
    }
    return res;
}
