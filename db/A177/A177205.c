/*
 * A177205  Partial sums of round(n^2/17).
 * https://oeis.org/A177205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + n + 36)/102)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 890, u64 n <= 890,
 *            u128 n <= 890, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partial sums of round(k^2/17) for k = 0..n */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;
    seq_t n_cu = n_sq * nn;
    seq_t temp = (seq_t)2 * n_cu + (seq_t)3 * n_sq + nn + (seq_t)36;
    return temp / (seq_t)102;
}
