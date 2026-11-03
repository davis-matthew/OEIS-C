/*
 * A024081  a(n) = 7^n - n^6.
 * https://oeis.org/A024081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 396 (2026-11-01)
 * Helpers:   ipow_i32, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow7 = ipow_u64_idx(7ULL, n);      /* compute 7^n */
    seq_t pow6 = ipow_i32((seq_t)n, 6);   /* compute n^6 */
    return pow7 - pow6;
}
