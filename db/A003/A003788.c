/*
 * A003788  Order of universal Chevalley group A_n (4).
 * https://oeis.org/A003788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n*(n+1)/2) * Product_{i=2..n+1} (4^i - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    idx_t e = n * (n + 1) / 2;
    seq_t result = ipow_idx((seq_t)4, e);
    seq_t p = (seq_t)16;
    idx_t i;
    for (i = 2; i <= n + 1; i++) {
        result *= (p - (seq_t)1);
        p *= (seq_t)4;
    }
    return result;
}
