/*
 * A227935  G.f.: Sum_{n>=0} x^n / (1-x)^(n^5).
 * https://oeis.org/A227935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k^5 + n - k - 1, n - k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset n = 0 explicitly – the sum over k contains the
       term binomial(-1,0) which is defined as 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k)         /* k = 0 gives no contribution for n > 0 */
    {
        seq_t k_seq = (seq_t)k;
        seq_t k5 = big_pow(k_seq, (seq_t)5);   /* k^5 */

        seq_t N;
        if (k == n)                          /* n - k - 1 == -1 in this case */
            N = k5 - (seq_t)1;
        else                                  /* n - k - 1 is non‑negative when k < n */
            N = k5 + (seq_t)(n - k - 1);

        idx_t K = n - k;                     /* choose K from N */

        seq_t bin = 1;
        /* Compute C(N,K) multiplicatively:
           bin = Π_{i=1..K} (N - K + i) / i           */
        for (idx_t i = 1; i <= K; ++i)
        {
            seq_t term = N - (seq_t)(K - i);
            bin = (bin * term) / i;              /* division is exact */
        }

        sum += bin;
    }

    return sum;
}
