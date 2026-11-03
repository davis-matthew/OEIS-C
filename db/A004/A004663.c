/*
 * A004663  Powers of 3 written in base 9.
 * https://oeis.org/A004663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^(n mod 2) * 10^(n/2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 1780 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t base = ipow_idx(10, n / 2);
    return (n & 1) ? 3 * base : base;
}
