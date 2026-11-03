/*
 * A074623  a(n) = 7^n + 9^n.
 * https://oeis.org/A074623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

/* OEIS A074623: a(n) = 7^n + 9^n */
seq_t a(idx_t n)
{
    return ipow_idx(7, n) + ipow_idx(9, n);
}
