/*
 * A073397  Eighth convolution of A002605(n) (generalized (2,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n‑k+8,8) * binomial(n‑k,k) *
 *            pow2(n‑k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 28, u128 n <= 66,
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
    idx_t half = n >> 1;       /* floor(n/2) */
    for (idx_t k = 0; k <= half; ++k)
    {
        idx_t nk = n - k;
        seq_t term = binomial(nk + 8, 8);
        term *= binomial(nk, k);
        term *= pow2(nk);
        sum += term;
    }
    return sum;
}
