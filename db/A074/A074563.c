/*
 * A074563  a(n) = 4^n + 5^n + 8^n.
 * https://oeis.org/A074563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 945 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p4 = ipow_u64_idx(4, n);
    seq_t p5 = ipow_u64_idx(5, n);
    seq_t p8 = ipow_u64_idx(8, n);
    return p4 + p5 + p8;
}
