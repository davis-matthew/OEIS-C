/*
 * A006236  a(n) = n^(n-2)*(n+2)^(n-1).
 * https://oeis.org/A006236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2)*(n+2)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 99 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n < 2) {
        return (seq_t)1;
    }
    seq_t nv = (seq_t)n;
    seq_t np2 = nv + 2;
    return ipow_idx(nv, n - 2) * ipow_idx(np2, n - 1);
}
