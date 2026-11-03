/*
 * A178358  Rounded up arithmetic mean of digits of n prepended to n, cf.
 *          A004427.
 * https://oeis.org/A178358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = ceil( digit_sum(n) / digits(n) ) * 10^digits(n) + n
 * Range:     u8 n <= 13, u16 n <= 1698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8762 (2026-11-01)
 * Helpers:   digit_sum, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/num_digits_idx.h"

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
    /* number of decimal digits */
    int d = num_digits_idx(n);
    if (d <= 0) d = 1;                     /* protects against 0, though digits(0) = 1 */
    /* sum of digits */
    seq_t sum = digit_sum((seq_t)n);
    /* ceil of the average */
    seq_t avg = (sum + d - 1) / d;
    /* 10^digits */
    seq_t tpow = pow10((idx_t)d);
    /* final result */
    return avg * tpow + (seq_t)n;
}
