/*
 * A013821  a(n) = 24^(4*n + 3).
 * https://oeis.org/A013821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(4*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 96 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t exponent = ((seq_t)n * 4) + 3;
    return ipow((seq_t)24, exponent);
}
