/*
 * A054338  8-fold convolution of A000302 (powers of 4).
 * https://oeis.org/A054338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,7) * 4^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 391 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

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
    /* Compute the binomial coefficient binom(n+7,7) */
    seq_t bin = binomial(n + 7, (idx_t)7);

    /* Compute 4^n */
    seq_t p4 = pow4(n);

    /* Result is the product */
    return bin * p4;
}
