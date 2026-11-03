/*
 * A155597  a(n) = 6^n - 2^n + 1.
 * https://oeis.org/A155597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - 2^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow_idx((seq_t)6, n);
    seq_t pow2 = ipow_idx((seq_t)2, n);
    return pow6 - pow2 + (seq_t)1;
}
