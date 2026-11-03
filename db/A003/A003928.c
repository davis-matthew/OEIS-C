/*
 * A003928  Order of simple Chevalley group B_n (5).
 * https://oeis.org/A003928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n^2) * Product_{i=1..n} (5^(2i) - 1) / 2 (order of
 *            B_n(5), gcd(2,5-1) = 2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t product = (seq_t)12; /* (5^2 - 1) / 2 */
    seq_t p = (seq_t)25;
    idx_t i;
    for (i = 2; i <= n; i++) {
        p = p * (seq_t)25;
        product = product * (p - (seq_t)1);
    }
    return ipow_idx((seq_t)5, n * n) * product;
}
