/*
 * A072975  a(n) = 2^n*binomial(3*n,n)*(3*n+1).
 * https://oeis.org/A072975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * binomial(3*n, n) * (3*n + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 3*n as idx_t */
    idx_t three_n = 3 * n;

    /* binomial(3*n, n) as seq_t */
    seq_t bin = binomial(three_n, n);

    /* (3*n+1) as seq_t */
    seq_t three_n_plus1 = (seq_t)(three_n + 1);

    /* bin * (3*n+1) */
    seq_t res = seq_mul(bin, three_n_plus1);

    /* multiply by 2^n */
    res = seq_mul(res, pow2(n));

    return res;
}
