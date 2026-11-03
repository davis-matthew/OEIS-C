/*
 * A238275  a(n) = (4*7^n - 1)/3.
 * https://oeis.org/A238275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*7^n - 1)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 196 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t val = ipow_u64_idx(7, n);
    val = val * 4;
    val = val - 1;
    val = val / 3;
    return val;
}
