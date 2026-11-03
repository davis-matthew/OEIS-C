/*
 * A097167  a(n) = 3*10^n - 2*9^n.
 * https://oeis.org/A097167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 948 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"
/*
 * Sequence: a(n) = 3*10^n - 2*9^n
 * (OEIS A097167)
 */

seq_t a(idx_t n) {
    seq_t pow10 = ipow_idx((seq_t)10, n);
    seq_t pow9  = ipow_idx((seq_t)9,  n);
    seq_t term10 = (seq_t)3 * pow10;
    seq_t term9  = (seq_t)2 * pow9;
    return term10 - term9;
}
