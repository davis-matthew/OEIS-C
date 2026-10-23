/*
 * A119365  Generalized Catalan numbers for triangle A119335.
 * https://oeis.org/A119365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} if ((n-k)%3==0, floor((C(n,k)*C(n,k+1))/n),
 *            0), with a(0)=1. Here C(a,b) is the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();    /* a(0) = 1 */

    seq_t n_seq = (seq_t)n;
    seq_t sum   = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if ( (n - k) % 3 == 0 )
        {
            seq_t c1 = binom(n, k);
            seq_t c2 = (k + 1 <= n) ? binom(n, k + 1) : (seq_t)0;
            seq_t prod = c1 * c2;
            prod = prod / n_seq;   /* exact division */
            sum += prod;
        }
    }
    return sum;
}
