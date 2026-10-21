/*
 * A073398  Ninth convolution of A002605(n) (generalized (2,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k+9, 9)*binomial(n-k,
 *            k)*2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n >> 1;          /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k) {
        idx_t nk = n - k;
        seq_t bin1 = binomial(nk + 9u, 9u);   /* binomial(n-k+9, 9) */
        seq_t bin2 = binomial(nk, k);         /* binomial(n-k, k)   */
        seq_t pow2_val = pow2(nk);            /* 2^(n-k)            */
        seq_t term = bin1 * bin2;
        term *= pow2_val;
        sum += term;
    }
    return sum;
}
