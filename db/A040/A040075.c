/*
 * A040075  5-fold convolution of A000302 (powers of 4); expansion of
 *          1/(1-4*x)^5.
 * https://oeis.org/A040075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+4,4) * 4^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 369 (2026-11-01)
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
    /* Compute (n+4 choose 4) * 4^n */
    return binomial(n + 4, 4) * pow4(n);
}
