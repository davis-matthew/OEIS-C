/*
 * A066210  a(n) = ((2*n)^(2*n+2) - 1)/(4*n^2 - 1).
 * https://oeis.org/A066210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)^(2*n+2) - 1)/(4*n^2 - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 98 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t base      = (seq_t)(2 * n);
    idx_t exponent  = (idx_t)(2 * (idx_t)n + 2);

    seq_t numerator = ipow_idx(base, exponent) - (seq_t)1;

    seq_t denom = 4 * (seq_t)n * (seq_t)n - (seq_t)1;

    return numerator / denom;
}
