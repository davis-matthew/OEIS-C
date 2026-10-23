/*
 * A034806  Number of distinct sets of 2 numbers > 1 such that their product
 *          is between n^2 and (n+1)^2.
 * https://oeis.org/A034806
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} floor((2*n + (n*n mod k))/k)
 * Range:     u8 n <= 15, u16 n <= 280, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* behaviour for n < 2 is irrelevant */

    seq_t sum   = 0;
    seq_t nseq  = (seq_t)n;
    seq_t n2    = nseq * nseq;

    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t mod  = n2 % kseq;
        seq_t term = (2 * nseq + mod) / kseq;
        sum += term;
    }
    return sum;
}
