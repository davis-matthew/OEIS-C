/*
 * A024062  a(n) = 6^n - 1.
 * https://oeis.org/A024062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 1 a(0) = 0, a(n+1) = 6*a(n) + 5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 462 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    seq_t p = ipow_u64_idx(6, n);
    return p - (seq_t)1;
}
