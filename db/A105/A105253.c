/*
 * A105253  a(n) = binomial(n+6,n)*binomial(n+10,n).
 * https://oeis.org/A105253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,n)*binomial(n+10,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 57, u128 n <= 987,
 *            big any n
 * Helpers:   gcd_any
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_any.h"

/* Compute binomial coefficient C(N,K) for idx_t N,K using only integer
   operations.  The algorithm reduces intermediate results by cancelling
   common factors with the current result to keep values small. */
static seq_t binom_n_k(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;

    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd_any(num, den);
        num /= g1;
        den /= g1;

        seq_t g2 = gcd_any(res, den);
        res /= g2;
        den /= g2;

        /* After cancellations, den should be 1.  If not, it divides res. */
        res *= num;
        res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t b1 = binom_n_k(n + 6, n);
    seq_t b2 = binom_n_k(n + 10, n);
    return b1 * b2;
}
