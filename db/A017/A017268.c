/*
 * A017268  a(n) = (9*n + 8)^12.
 * https://oeis.org/A017268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 3, u128 n <= 179,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 9935 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * (seq_t)9 + (seq_t)8;
    return ipow(m, (seq_t)12);
}
