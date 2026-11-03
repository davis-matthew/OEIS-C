/*
 * A060613  Number of n X n {-1,0,1} matrices with no zero rows.
 * https://oeis.org/A060613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^n - 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 44 (2026-11-01)
 * Helpers:   ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow3 = ipow_u64_idx(3, n);
    seq_t base = pow3 - 1;
    seq_t result = ipow_i32(base, (int)n);
    return result;
}
