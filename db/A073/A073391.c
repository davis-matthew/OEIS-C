/*
 * A073391  Fifth column of the convolution triangle A073387(n+4 4), for n >=
 *          0.
 * https://oeis.org/A073391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋}
 *            binomial(n-k+4,4)*binomial(n-k,k)*2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 34, u128 n <= 75,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 2;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t nk = n - k;
        seq_t bin1 = binom(nk + 4, 4);
        seq_t bin2 = binom(nk, k);
        seq_t powv = pow2(nk);
        seq_t term = bin1;
        term *= bin2;
        term *= powv;
        sum += term;
    }
    return sum;
}
