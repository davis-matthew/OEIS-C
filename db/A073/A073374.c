/*
 * A073374  Fourth convolution of A001045(n+1) (generalized (1,2)-Fibonacci),
 *          n>=0, with itself.
 * https://oeis.org/A073374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binom(n-k+4,4) * binom(n-k,k) * 2^k
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 48, u128 n <= 108,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t half = n >> 1;          /* floor(n/2) */
    seq_t sum  = 0;
    seq_t pow2 = 1;               /* 2^k for current k */

    for (idx_t k = 0; k <= half; ++k)
    {
        idx_t nk   = n - k;                                   /* n−k */
        seq_t bin1 = binom(nk, k);                            /* C(n‑k, k) */
        seq_t bin2 = binom(nk + 4, 4);                        /* C(n‑k+4, 4) */

        seq_t term = bin1;
        term *= bin2;                                         /* bin1 * bin2 */
        term *= pow2;                                         /* * 2^k */

        sum += term;                                          /* accumulate */

        pow2 <<= 1;                                           /* next power of two */
    }

    return sum;
}
