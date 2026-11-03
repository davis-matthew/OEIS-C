/*
 * A017275  a(n) = (10*n)^7.
 * https://oeis.org/A017275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9425 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    /* Calculate (10 * n)^7 using the provided seq_pow helper */
    seq_t base = (seq_t)10 * n;
    return ipow_idx(base, 7U);
}
