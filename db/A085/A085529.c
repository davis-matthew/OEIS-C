/*
 * A085529  a(n) = (2n+1)^(2n+1).
 * https://oeis.org/A085529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)^(2n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(2 * n + 1);
    return ipow_idx(base, 2 * n + 1);
}
