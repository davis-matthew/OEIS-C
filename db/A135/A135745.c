/*
 * A135745  E.g.f.: A(x) = Sum_{n>=0} exp((n-1)*x)^n * x^n/n!.
 * https://oeis.org/A135745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binom(n,k) * [k*(k-1)]^(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0 or 1 the sum is 1 because k = 0 or 1 contribute only to a(0) and a(1). */

#include "../utils/big_pow.h"
#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 2; k <= n; ++k)
    {
        seq_t base = ((seq_t)k) * (((seq_t)k) - 1);
        seq_t term = big_pow(base, (seq_t)(n - k));
        term *= binom_n_k(n, k);
        sum += term;
    }
    return sum;
}
