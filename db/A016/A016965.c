/*
 * A016965  a(n) = (6*n + 4)^9.
 * https://oeis.org/A016965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^9
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 22, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 944 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 6 + 4;
    return ipow_i32(base, 9);
}
