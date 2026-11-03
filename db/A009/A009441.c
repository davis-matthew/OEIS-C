/*
 * A009441  a(n) is the concatenation of n and 7n.
 * https://oeis.org/A009441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = concat(n, 7n)
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 937 (2026-11-01)
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

seq_t a(idx_t n)
{
    seq_t seven_n = (seq_t)n * (seq_t)7;
    int k = num_digits(seven_n);
    seq_t pow = pow10((idx_t)k);
    return (seq_t)n * pow + seven_n;
}
