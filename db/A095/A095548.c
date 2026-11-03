/*
 * A095548  a(n) = 78 written in base n.
 * https://oeis.org/A095548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = base‑n representation of the decimal number 78 * (digits
 *            concatenated into a decimal integer; digits ≥10 are * converted
 *            to their decimal values before concatenation)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Special case n == 1 : unary representation
     * (78 copies of the digit 1). */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 78; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Compute digits of 78 in base n. */
    seq_t const val = 78;
    seq_t digits[128];
    size_t cnt = 0;
    seq_t tmp = val;
    do {
        digits[cnt++] = tmp % n;
        tmp /= n;
    } while (tmp != 0);

    /* Build the concatenated decimal integer. */
    seq_t res = 0;
    for (size_t i = cnt; i > 0; --i) {
        seq_t d = digits[i - 1];
        int len = num_digits(d);           /* decimal length of d */
        res = res * pow10((idx_t)len) + d;
    }
    return res;
}
