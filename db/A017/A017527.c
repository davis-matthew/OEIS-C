/*
 * A017527  a(n) = (12*n)^7.
 * https://oeis.org/A017527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 47, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 9896 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 12;
    return ipow(base, 7);
}
