/*
 * A082243  a(n) = A082242(n)/4.
 * https://oeis.org/A082243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat(k, k+1, k+2, k+3) / 4 * where k = 3 for n = 1 * k
 *            = 4n+1 for n ≥ 2 * (The concatenation is a decimal string formed
 *            by the four numbers.)
 * Range:     u8 none, u16 n <= 1, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t start;
    if (n == (idx_t)1)
        start = (seq_t)3;
    else
        start = (seq_t)(4 * n + (idx_t)1);

    seq_t res = 0;
    for (int i = 0; i < 4; i++) {
        seq_t cur = start + (seq_t)i;
        unsigned len = num_digits(cur);
        res = res * pow10(len) + cur;
    }
    return res / (seq_t)4;
}
