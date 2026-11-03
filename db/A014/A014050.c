/*
 * A014050  a(n) = (n^2+1)^n.
 * https://oeis.org/A014050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2+1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 199 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)n + 1;
    return ipow_idx(base, n);
}
