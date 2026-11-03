/*
 * A024098  a(n) = 8^n - n^10.
 * https://oeis.org/A024098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 8, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 382 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t term8  = ipow_u64_idx(8ULL, n);          /* compute 8^n   */
    seq_t term10 = ipow_u64_idx((uint64_t)n, 10ULL); /* compute n^10 */
    return term8 - term10;
}
