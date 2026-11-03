/*
 * A007781  a(n) = (n+1)^(n+1) - n^n for n>0, a(0) = 1.
 * https://oeis.org/A007781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^(n+1) - n^n for n>0, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 93 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    
    seq_t n_seq = (seq_t)n;
    seq_t term1 = ipow_idx(n_seq + 1, n + 1);
    seq_t term2 = ipow_idx(n_seq, n);
    
    return term1 - term2;
}
