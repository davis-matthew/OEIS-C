/*
 * A013711  a(n) = 6^(2*n+1).
 * https://oeis.org/A013711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 178 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n * 2 + 1;
    return ipow((seq_t)6, exp);
}
