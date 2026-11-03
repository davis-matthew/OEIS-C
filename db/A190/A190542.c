/*
 * A190542  a(n) = 7^n - 4^n.
 * https://oeis.org/A190542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 4^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 296 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computed by direct exponentiation using pow_uint() */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow7 = ipow_u64_idx(7, n);
    seq_t pow4 = ipow_u64_idx(4, n);
    return pow7 - pow4;
}
