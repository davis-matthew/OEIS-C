/*
 * A008977  a(n) = (4*n)!/(n!)^4.
 * https://oeis.org/A008977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (4*n)!/(n!)^4 = C(2*n,n) * C(3*n,n) * C(4*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t v1 = binom(2*n, n);
    seq_t v2 = seq_mul(v1, binom(3*n, n));
    seq_t v3 = seq_mul(v2, binom(4*n, n));
    return v3;
}
