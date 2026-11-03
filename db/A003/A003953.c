/*
 * A003953  Expansion of g.f.: (1+x)/(1-10*x).
 * https://oeis.org/A003953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 11*10^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 186 (2026-11-01)
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
    return (seq_t)11 * ipow_idx((seq_t)10, n - 1);
}
