/*
 * A074561  a(n) = 4^n + 5^n + 6^n.
 * https://oeis.org/A074561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 861 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t term4 = ipow_u64_idx(4, n);
    seq_t term5 = ipow_u64_idx(5, n);
    seq_t term6 = ipow_u64_idx(6, n);
    return term4 + term5 + term6;
}
