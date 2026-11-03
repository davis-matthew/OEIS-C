/*
 * A022534  Nexus numbers (n+1)^18 - n^18.
 * https://oeis.org/A022534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^18 - n^18
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 10, u128 n <= 155,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 9570 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t base1 = (seq_t)n + 1;
    seq_t p1 = ipow_idx(base1, 18);
    seq_t p2 = ipow_idx((seq_t)n, 18);
    return p1 - p2;
}
