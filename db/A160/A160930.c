/*
 * A160930  a(n) = n + reversal(n-1) + reversal(n+1).
 * https://oeis.org/A160930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + rev(n-1) + rev(n+1)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9058 (2026-11-01)
 * Helpers:   reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/reverse_digits_idx.h"

seq_t a(idx_t n)
{
    idx_t n_minus = n - 1;          /* n ≥ 1 guarantees no underflow */
    seq_t res   = (seq_t)n;
    seq_t rev1  = (seq_t)reverse_digits_idx(n_minus);
    seq_t rev2  = (seq_t)reverse_digits_idx(n + 1);
    return res + rev1 + rev2;
}
