/*
 * A076008  Second column of triangle A075504.
 * https://oeis.org/A076008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*18^n - 9^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 794 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow9  = ipow_u64_idx(9,  n);
    seq_t pow18 = ipow_u64_idx(18, n);
    return pow18 * 2u - pow9;
}
