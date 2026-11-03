/*
 * A006127  a(n) = 2^n + n.
 * https://oeis.org/A006127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 99 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    return pow2(n) + (seq_t)n;
}
