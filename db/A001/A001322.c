/*
 * A001322  Number of equivalence classes of n-valued Post functions of 2
 *          variables under action of symmetric group S_2.
 * https://oeis.org/A001322
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n^2) + n^(n(n+1)/2)) / 2, computed as n^m * (n^k + 1)
 *            / 2 with m = n(n+1)/2, k = n(n-1)/2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 8 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    idx_t m = n * (n + 1) / 2;
    idx_t k = n * (n - 1) / 2;
    if (n % 2 == 0) {
        seq_t h = ((seq_t)(n / 2)) * ipow_idx(nn, m - 1);
        return h * (ipow_idx(nn, k) + 1);
    } else {
        seq_t q = (ipow_idx(nn, k) + 1) / 2;
        return ipow_idx(nn, m) * q;
    }
}
