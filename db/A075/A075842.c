/*
 * A075842  n 1's followed by n.
 * https://oeis.org/A075842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = ((10^n - 1)/9) * 10^{digits(n)} + n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    /* Construct (10^n - 1)/9 */
    seq_t ones = pow10(n);
    ones -= (seq_t)1;
    ones /= (seq_t)9;

    /* Multiply by 10^{digits(n)} and add n */
    int L = num_digits_idx(n);
    seq_t powL = pow10((idx_t)L);
    seq_t res = ones * powL;
    res += (seq_t)n;
    return res;
}
