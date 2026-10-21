/*
 * A240165  E.g.f.: exp( x*(1 + exp(2*x)) ).
 * https://oeis.org/A240165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (2*k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);
        seq_t base = (seq_t)(2 * k + 1);
        seq_t pw   = big_pow(base, (seq_t)(n - k));   /* (2k+1)^(n-k) */
        sum += bin * pw;
    }
    return sum;
}
