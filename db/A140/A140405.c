/*
 * A140405  a(n) = binomial(n+6, 6)*5^n.
 * https://oeis.org/A140405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 6, 6) * 5^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute binomial coefficient C(n+6, 6) */
    seq_t bin = binomial(n + 6, 6);

    /* Compute 5^n */
    seq_t pow5 = ipow_u64_idx(5, n);

    /* Return the product */
    return bin * pow5;
}
