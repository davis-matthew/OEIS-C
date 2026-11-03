/*
 * A017424  a(n) = (11*n + 2)^12.
 * https://oeis.org/A017424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9614 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = 11 * (seq_t)n + 2;
    return ipow(base, (seq_t)12);
}
