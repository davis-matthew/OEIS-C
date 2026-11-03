/*
 * A024074  a(n) = 6^n - n^12.
 * https://oeis.org/A024074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 467 (2026-11-01)
 * Helpers:   ipow, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = ipow((seq_t)6, (seq_t)n);
    
    /* n^12 */
    seq_t pown12 = ipow_idx((seq_t)n, 12);
    
    return pow6 - pown12;
}
