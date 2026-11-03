/*
 * A008455  11th powers: a(n) = n^11.
 * https://oeis.org/A008455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 994 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;
    return ipow(base, (seq_t)11);
}
