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
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse.h"

seq_t a(idx_t n)
{
    seq_t r1 = reverse(n);
    seq_t r2 = reverse(n + 1);
    return r1 * r2;
}
