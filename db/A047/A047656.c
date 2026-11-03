/*
 * A047656  a(n) = 3^((n^2-n)/2).
 * https://oeis.org/A047656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^((n^2 - n)/2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 65 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    idx_t exp_idx = (idx_t)n * ((idx_t)n - 1) / 2;
    return ipow((seq_t)3, (seq_t)exp_idx);
}
