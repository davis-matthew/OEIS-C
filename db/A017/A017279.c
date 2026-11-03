/*
 * A017279  a(n) = (10*n)^11.
 * https://oeis.org/A017279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 318,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 387 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)10;
    return ipow(base, (seq_t)11);
}
