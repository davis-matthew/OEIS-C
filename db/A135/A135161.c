/*
 * A135161  a(n) = 7^n - 5^n - 3^n - 2^n. Constants are the prime numbers in
 *          decreasing order.
 * https://oeis.org/A135161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 7^n - 5^n - 3^n - 2^n
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 987 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p7 = ipow_u64_idx(7, n);
    seq_t p5 = ipow_u64_idx(5, n);
    seq_t p3 = ipow_u64_idx(3, n);
    seq_t p2 = ipow_u64_idx(2, n);
    return p7 - p5 - p3 - p2;
}
