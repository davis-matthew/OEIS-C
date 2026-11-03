/*
 * A053132  One half of binomial coefficients C(2*n-4,5).
 * https://oeis.org/A053132
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-4,5)/2 (for n ≥ 5, otherwise 0)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 119, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 47 terms through n = 187 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;
    }

    /* 2*n-4 fits in idx_t for all n relevant to this sequence. */
    idx_t m = 2 * n - 4;

    /* binom returns a value of type seq_t; the result is always even 
       so the division by 2 is exact. */
    return binomial(m, 5) / 2;
}
