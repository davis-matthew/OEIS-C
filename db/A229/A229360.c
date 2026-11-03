/*
 * A229360  Total sum of n-th powers of parts in all partitions of 9.
 * https://oeis.org/A229360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 67 + 26*2^n + 15*3^n + 8*4^n + 5*5^n + 3*6^n + 2*7^n +
 *            8^n + 9^n
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 474 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t res = 67;
    res += 26 * ipow_u64_idx(2, n);
    res += 15 * ipow_u64_idx(3, n);
    res += 8  * ipow_u64_idx(4, n);
    res += 5  * ipow_u64_idx(5, n);
    res += 3  * ipow_u64_idx(6, n);
    res += 2  * ipow_u64_idx(7, n);
    res += ipow_u64_idx(8, n);
    res += ipow_u64_idx(9, n);
    return res;
}
