/*
 * A119579  a(n) = (n + n^2)*(binomial(2*n, n)).
 * https://oeis.org/A119579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + n^2)*binomial(2*n, n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t factor = nn * (nn + 1);
    seq_t binom = binomial(n << 1, n);
    return factor * binom;
}
