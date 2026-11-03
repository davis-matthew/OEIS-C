/*
 * A001447  a(n) = (5^n + 5^floor(n/2))/2.
 * https://oeis.org/A001447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + 5^floor(n/2))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 294 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t power_n = ipow_idx((seq_t)5, n);
    seq_t power_half_n = ipow_idx((seq_t)5, n / 2);
    return (power_n + power_half_n) / 2;
}
