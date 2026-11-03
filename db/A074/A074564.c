/*
 * A074564  a(n) = 4^n + 5^n + 9^n.
 * https://oeis.org/A074564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 5^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 985 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t p4 = ipow_idx((seq_t)4, n);
    seq_t p5 = ipow_idx((seq_t)5, n);
    seq_t p9 = ipow_idx((seq_t)9, n);
    return p4 + p5 + p9;
}
