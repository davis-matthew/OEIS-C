/*
 * A017003  a(n) = (7*n + 1)^11.
 * https://oeis.org/A017003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 454,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 9503 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + 1;
    return ipow(base, (seq_t)11);
}
