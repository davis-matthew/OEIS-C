/*
 * A119576  a(n) = (n+n^2+n^3)*(binomial(2*n,n)).
 * https://oeis.org/A119576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+n^2+n^3)*C(2n,n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t tn  = (seq_t)n;
    seq_t tn2 = tn * tn;
    seq_t tn3 = tn2 * tn;
    seq_t factor = tn + tn2 + tn3;
    seq_t bin = binomial(2 * n, n);
    return factor * bin;
}
