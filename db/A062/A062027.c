/*
 * A062027  a(1) = a(2) = a(3) = 1 and a(n) = 24*binomial(n+1, 5) + n*(n^2 - n
 *          + 6) for n > 3.
 * https://oeis.org/A062027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*binomial(n+1,5) + n*(n^2 - n + 6), with
 *            a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the helper that computes binomial coefficients. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n <= 3)
        return 1;

    /* Compute 24 * C(n+1,5). */
    seq_t term1 = 24 * binom(n + 1, 5);

    /* Compute n * (n^2 - n + 6) using seq_t arithmetic. */
    seq_t nseq = (seq_t)n;
    seq_t term2 = nseq * (nseq * nseq - nseq + 6);

    return term1 + term2;
}
