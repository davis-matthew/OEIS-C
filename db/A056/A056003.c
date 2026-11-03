/*
 * A056003  a(n) = (n+1)*binomial(n+8, 8).
 * https://oeis.org/A056003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+8, 8)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 34, u64 n <= 445,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9836 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+8, 8) using the provided binom helper */
    seq_t bin = binomial(n + 8, 8);

    /* Multiply by (n+1) */
    seq_t result = (seq_t)(n + 1) * bin;

    return result;
}
