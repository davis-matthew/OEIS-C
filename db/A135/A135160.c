/*
 * A135160  a(n) = 5^n + 3^n - 2^n.
 * https://oeis.org/A135160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 3^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 822 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p5 = ipow_u64_idx(5, n);
    seq_t p3 = ipow_u64_idx(3, n);
    seq_t p2 = ipow_u64_idx(2, n);
    seq_t sum = p5 + p3;
    return sum - p2;
}
