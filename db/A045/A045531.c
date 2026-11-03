/*
 * A045531  Number of sticky functions: endofunctions of [n] having a fixed
 *          point.
 * https://oeis.org/A045531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^n - (n‑1)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* n must be >= 1 in the OEIS convention.
       If n == 0, return 0 as a harmless default. */
    if (n == 0) return 0;

    seq_t p_n   = ipow_idx((seq_t)n, n);          /* n^n   */
    seq_t p_n1  = ipow_idx((seq_t)(n - 1), n);    /* (n-1)^n */

    return p_n - p_n1;
}
