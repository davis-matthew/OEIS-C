/*
 * A198855  a(n) = 7*8^n - 1.
 * https://oeis.org/A198855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*8^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 929 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    return ipow_idx((seq_t)8, n) * (seq_t)7 - (seq_t)1;
}
