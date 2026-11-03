/*
 * A198688  a(n) = 6*7^n-1.
 * https://oeis.org/A198688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*7^n - 1, a(0) = 5; recurrence: a(n) = 7*a(n-1) + 6
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 993 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p = ipow_u64_idx(7, n);
    return p * 6 - 1;
}
