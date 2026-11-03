/*
 * A132934  Concatenation of first n nonprime numbers.
 * https://oeis.org/A132934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 359 (2026-11-01)
 * Helpers:   is_prime, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Concatenation of the first n nonprime numbers.
   a(1) = 1, a(2) = 14, a(3) = 146, … */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; safe fallback */
    seq_t res   = 0;
    idx_t count = 0;
    idx_t x     = 1;

    while (count < n) {
        if (!is_prime(x)) {        /* x is not prime (includes 1) */
            int d = num_digits_idx(x); /* decimal digit count of x */
            res = seq_mul(res, pow10(d)) + x;
            ++count;
        }
        ++x;
    }
    return res;
}
