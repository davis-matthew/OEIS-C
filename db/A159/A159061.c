/*
 * A159061  Nearest integer to the expected number of tosses of a fair coin
 *          required to obtain at least n heads and n tails.
 * https://oeis.org/A159061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nearest integer to 2*n*(1 + binom(2*n,n)/2^(2*n))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute the central binomial probability
       p = binom(2n,n) / 4^n  iteratively to avoid overflow. */
    long double p = 1.0L;
    for (idx_t i = 1; i <= n; ++i) {
        p *= ((long double)(n + i)) / (4.0L * (long double)i);
    }

    /* Compute the fractional part 2n*p and round to nearest integer. */
    long double rnd = 2.0L * (long double)n * p;
    unsigned long long inc = (unsigned long long)floorl(rnd + 0.5L);

    /* a(n) = 2n + round(2n*p) */
    seq_t res = (seq_t)((seq_t)2 * (seq_t)n + (seq_t)inc);
    return res;
}
