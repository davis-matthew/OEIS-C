/*
 * A204692  a(n) is the number of base-10 bouncy numbers below 10^n.
 * https://oeis.org/A204692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^n - binomial(n+10,10) - binomial(n+9,9) + 10*n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 195 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    if (n < 3) return 0;
    seq_t pow10_n   = pow10(n);
    seq_t bin10     = binomial(n + 10, 10);
    seq_t bin9      = binomial(n + 9,  9);
    seq_t result    = pow10_n + (seq_t)n * 10 + 1;
    result = result - bin10 - bin9;
    return result;
}
