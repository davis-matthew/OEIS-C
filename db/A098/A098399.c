/*
 * A098399  a(n) = 3^n*binomial(2*n+1, n).
 * https://oeis.org/A098399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^n * binomial(2*n+1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* 3^n, computed iteratively to avoid overflow of intermediate
       values beyond seq_t for the exponent range used. */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* binomial(2*n+1, n) uses the provided exact combinatorial routine. */
    idx_t two_n_plus_one = n * 2 + 1;
    seq_t bin = binomial(two_n_plus_one, n);

    return pow3 * bin;
}
