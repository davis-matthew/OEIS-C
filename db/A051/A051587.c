/*
 * A051587  Number of 4 X n binary matrices such that any 2 rows have a common
 *          1.
 * https://oeis.org/A051587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n - 6*12^n + 12*10^n - 9^n - 16*8^n + 15*7^n - 6*6^n +
 *            5^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 812 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p16 = ipow_u64_idx(16, n);
    seq_t p12 = ipow_u64_idx(12, n);
    seq_t p10 = ipow_u64_idx(10, n);
    seq_t p9  = ipow_u64_idx(9,  n);
    seq_t p8  = ipow_u64_idx(8,  n);
    seq_t p7  = ipow_u64_idx(7,  n);
    seq_t p6  = ipow_u64_idx(6,  n);
    seq_t p5  = ipow_u64_idx(5,  n);

    /* Sum of positive terms */
    seq_t sum_pos = p16;
    sum_pos += p10 * (seq_t)12;
    sum_pos += p7  * (seq_t)15;
    sum_pos += p5;

    /* Sum of negative terms */
    seq_t sum_neg = p12 * (seq_t)6;
    sum_neg += p9;
    sum_neg += p8  * (seq_t)16;
    sum_neg += p6  * (seq_t)6;

    /* a(n) = sum_pos - sum_neg (always non‑negative) */
    return sum_pos - sum_neg;
}
