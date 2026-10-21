/*
 * A079678  a(n) = a(n,m) = Sum_{k=0..n} binomial(m*k,k)*binomial(m*(n-k),n-k)
 *          for m=5.
 * https://oeis.org/A079678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(5*k,k)*binomial(5*(n-k),n-k)
 *            Computation uses the equivalent form a(n) = Sum_{k=0..n} 4^(n-k)
 *            * binomial(5*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Belongs to sequence A079678. */

#include "../utils/binomial.h"   /* provides binomial(idx_t, idx_t) */
#include "../utils/pow_uint.h"   /* provides pow_uint(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* 4^n as seq_t */
    seq_t pow4 = pow_uint(4, n);

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = 4^(n-k) * binomial(5*n+1, k) */
        seq_t term = pow4 * binomial(5 * n + 1, k);
        sum += term;

        /* prepare next power: 4^(n-(k+1)) = 4^(n-k) / 4 */
        if (k < n)
            pow4 /= 4;
    }
    return sum;
}
