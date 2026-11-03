/*
 * A102069  Analogous to the oblong (promic or heteromecic) sequence formed
 *          but with reversal digits of factors multiplied.
 * https://oeis.org/A102069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = R(n) * R(n+1) where R(n) is the decimal digit reversal of
 *            n
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   reverse_digits_idx_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits_idx_to_seq.h"

seq_t a(idx_t n)
{
    seq_t r1 = reverse_digits_idx_to_seq(n);
    seq_t r2 = reverse_digits_idx_to_seq(n + 1);
    return r1 * r2;
}
