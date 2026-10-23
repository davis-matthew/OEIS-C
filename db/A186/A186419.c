/*
 * A186419  a(n) = binomial(2*n,n)^4/(n + 1).
 * https://oeis.org/A186419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n, n)^4/(n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c = binomial((idx_t)(2 * n), n);
    seq_t r = c * c * c * c;
    return r / ((seq_t)n + 1);
}
