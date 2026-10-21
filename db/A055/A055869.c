/*
 * A055869  a(n) = (n+1)^n - n^n.
 * https://oeis.org/A055869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n - n^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t base1 = nn + 1;
    seq_t p1 = _pow_seq_t(base1, n);
    seq_t p2 = _pow_seq_t(nn, n);
    return p1 - p2;
}
