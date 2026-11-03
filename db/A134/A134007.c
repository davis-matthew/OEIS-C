/*
 * A134007  a(n) = 1^n + 3^n + 5^n + 7^n + 9^n.
 * https://oeis.org/A134007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 3^n + 5^n + 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 286 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    sum += ipow_u64_idx(1, n);
    sum += ipow_u64_idx(3, n);
    sum += ipow_u64_idx(5, n);
    sum += ipow_u64_idx(7, n);
    sum += ipow_u64_idx(9, n);
    return sum;
}
