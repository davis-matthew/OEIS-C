/*
 * A053126  Binomial coefficients binomial(2*n-3,4).
 * https://oeis.org/A053126
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-3, 4) for n >= 4, a(n) = 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 48 terms through n = 172 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    /* 2*n-3 fits in idx_t; binom works with idx_t arguments */
    return binomial(2 * n - 3, 4);
}
