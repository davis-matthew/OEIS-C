/*
 * A024039  a(n) = 4^n - n^3.
 * https://oeis.org/A024039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n^3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 479 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow4 = ipow_u64_idx(4, n);
    seq_t n_seq = (seq_t)n;
    seq_t n_cube = n_seq * n_seq * n_seq;
    return pow4 - n_cube;
}
