/*
 * A090023  Number of distinct lines through the origin in the n-dimensional
 *          lattice of side length 7.
 * https://oeis.org/A090023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n - 4^n - 3^n - 2^n + 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* Products: 8^n, 4^n, 3^n, 2^n */
    seq_t p8 = ipow_idx((seq_t)8, n);
    seq_t p4 = ipow_idx((seq_t)4, n);
    seq_t p3 = ipow_idx((seq_t)3, n);
    seq_t p2 = ipow_idx((seq_t)2, n);

    /* Sum of the subtracted terms */
    seq_t sum = p4 + p3 + p2;

    /* Result: 8^n + 2 - (4^n + 3^n + 2^n) */
    return p8 + (seq_t)2 - sum;
}
