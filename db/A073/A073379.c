/*
 * A073379  Ninth convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(n-k+9,9) * binomial(n-k,k) * 2^k
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 38, u128 n <= 92,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the pre‑existing util function binomial(idx_t, idx_t). */

#include "../utils/binomial.h"   /* provides binomial */

seq_t a(idx_t n)
{
    idx_t kmax = n / 2;          /* floor(n/2)   */
    seq_t total = 0;

    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t t1   = binomial(n - k + 9, 9);   /* C(n-k+9, 9)   */
        seq_t t2   = binomial(n - k, k);       /* C(n-k, k)     */
        seq_t pow2 = (seq_t)1 << k;            /* 2^k           */
        total += t1 * t2 * pow2;
    }
    return total;
}
