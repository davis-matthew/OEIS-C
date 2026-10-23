/*
 * A173148  a(n) = cos(2*n*arccos(sqrt(n))).
 * https://oeis.org/A173148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n,2*k)*(n-1)^(n-k)*n^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial first two values */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute (n-1)^n */
    seq_t pow1 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow1 = pow1 * (seq_t)(n - 1);

    seq_t sum  = pow1;          /* term for k = 0 */
    seq_t binom = 1;            /* binomial(2n,0) */
    seq_t pow2  = 1;            /* n^0 */

    /* Accumulate the remaining terms k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial coefficient:
           binom(new) = binom(old) * (2n-2(k-1))*(2n-2(k-1)-1) / ((2k)*(2k-1)) */
        seq_t a1 = (seq_t)(2 * n - 2 * (k - 1));
        seq_t a2 = (seq_t)(2 * n - 2 * (k - 1) - 1);
        seq_t d1 = (seq_t)(2 * k);
        seq_t d2 = (seq_t)(2 * k - 1);

        binom = binom * a1;
        binom = binom * a2;
        binom = binom / d1;
        binom = binom / d2;

        /* Update powers */
        pow1 = pow1 / (seq_t)(n - 1);   /* (n-1)^(n-k)   */
        pow2 = pow2 * (seq_t)n;         /* n^k           */

        /* Compute current term and add to sum */
        seq_t term = binom * pow1;
        term = term * pow2;
        sum = sum + term;
    }

    return sum;
}
