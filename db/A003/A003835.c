/*
 * A003835  Order of universal Chevalley group D_n (8).
 * https://oeis.org/A003835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n*(n-1)) * (8^n - 1) * Product_{i=1..n-1} (8^(2*i) -
 *            1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    const seq_t q = 8;
    seq_t result = ipow_idx(q, n * (n - 1));
    seq_t p2 = 1;   /* q^(2i) */
    idx_t i;

    result = result * (ipow_idx(q, n) - 1);
    for (i = 1; i < n; i++) {
        p2 = p2 * 64;
        result = result * (p2 - 1);
    }
    return result;
}
