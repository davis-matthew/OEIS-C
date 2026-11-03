/*
 * A130741  a(n) = 4^(4^n) + 3.
 * https://oeis.org/A130741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(4^n) + 3; offset 0
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 5 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute 4^n as a seq_t (the exponent for the outer power) */
    seq_t exp = pow4((idx_t)n);

    /* Compute 4^(4^n) using the arbitrary-precision power function */
    seq_t val = ipow((seq_t)4, exp);

    /* Add 3 to get the final term */
    val += (seq_t)3;

    return val;
}
