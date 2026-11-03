/*
 * A024073  a(n) = 6^n - n^11.
 * https://oeis.org/A024073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 498 (2026-11-01)
 * Helpers:   ipow_idx, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = ipow_u64_idx(6, n);

    /* n^11 */
    seq_t pown = ipow_idx((seq_t)n, 11);

    /* a(n) = 6^n - n^11 */
    return pow6 - pown;
}
