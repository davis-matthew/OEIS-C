/*
 * A002700  Coefficients of Chebyshev polynomials: n*(2*n+1) * 4^(n-1).
 * https://oeis.org/A002700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+1) * 4^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 585 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = n_seq * ((seq_t)2 * n_seq + (seq_t)1);
    seq_t power_of_4 = ipow_idx((seq_t)4, n - 1);
    return term1 * power_of_4;
}
