/*
 * A120799  a(n) = 3^(n^2)-2^n.
 * https://oeis.org/A120799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n^2) - 2^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 40 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* exponent for 3 is n^2, compute safely in seq_t then cast to idx_t */
    idx_t exp3 = (idx_t)((seq_t)n * (seq_t)n);
    seq_t pow3 = ipow_u64_idx(3, exp3);
    seq_t pow2 = ipow_u64_idx(2, n);
    return pow3 - pow2;
}
