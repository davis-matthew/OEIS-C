/*
 * A098402  a(n) = (0^n + 4^n * binomial(2*n,n))/2.
 * https://oeis.org/A098402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (0^n + 4^n * binomial(2*n,n))/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n=0 this gives 1; for n>0 the term reduces to 2^(2*n-1) * binomial(2*n,n) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* 0^0 = 1, 4^0 = 1, binom(0,0)=1 => (1+1)/2 = 1 */

    /* Compute binomial coefficient C(2n, n) */
    idx_t two_n = 2 * n;
    seq_t binom = binomial(two_n, n);

    /* Compute 2^(2n-1) by repeated multiplication; 2^(2n-1) = 4^n / 2 for n>0 */
    idx_t exp = two_n - 1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= (seq_t)2;

    return binom * pow2;
}
