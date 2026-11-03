/*
 * A016795  a(n) = (3n+2)^7.
 * https://oeis.org/A016795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n+2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t base  = 3 * n_seq + 2;
    return ipow_idx(base, 7);
}
