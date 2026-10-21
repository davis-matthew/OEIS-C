/*
 * A219706  Total number of nonrecurrent elements in all functions
 *          f:{1,2,...,n}->{1,2,...,n}.
 * https://oeis.org/A219706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{n-1} (n!/(n-k)!) * k * n^{n-k-1} * (n-k) where
 *            (n!/(n-k)!) is the falling factorial P(n,k). This summation uses
 *            only integer arithmetic and works for any build of seq_t
 *            (fixed‑size or arbitrary precision)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(0)=a(1)=0. */
    if (n <= 1)
        return 0;

    seq_t result = 0;
    seq_t n_seq  = (seq_t)n;          /* cast index to seq_t once */

    /* falling factorial P(n,1) = n. */
    seq_t falling = n_seq;

    /* pow_n = n^(n-2).  For n=2 this is n^0 = 1. */
    seq_t pow_n = big_pow(n_seq, (seq_t)(n - 2));

    for (idx_t k = 1; k <= n - 1; ++k) {
        /* term = falling * k * pow_n * (n-k) */
        seq_t term = falling * (seq_t)k;
        term = term * pow_n;
        term = term * (seq_t)(n - k);

        result += term;

        /* Update for next k, unless we are at the last iteration. */
        if (k < n - 1) {
            falling *= (seq_t)(n - k);   /* multiply by the next factor (n-k) */
            pow_n /= n_seq;              /* decrement exponent by 1 */
        }
    }

    return result;
}
