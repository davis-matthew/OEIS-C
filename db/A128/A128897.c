/*
 * A128897  a(n) = ((2n)^(2n-1)+1)/(2n+1).
 * https://oeis.org/A128897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)^(2*n-1)+1)/(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Compute 2*n as a seq_t value. */
    seq_t two_n = (seq_t)n * 2;

    /* Exponent: 2*n - 1.  Use idx_t for the exponent to match seq_pow. */
    idx_t exp = ((idx_t)n << 1) - 1;

    /* Denominator: 2*n + 1, also as a seq_t. */
    seq_t denom = two_n + 1;

    /* Numerator: (2*n)^(2*n-1) + 1. */
    seq_t num = ipow_idx(two_n, exp);
    num += 1;

    /* Return the integer division result. */
    return num / denom;
}
