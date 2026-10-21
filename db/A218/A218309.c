/*
 * A218309  E.g.f. A(x) satisfies A( x/(exp(3*x)*cosh(3*x)) ) =
 *          exp(4*x)*cosh(4*x).
 * https://oeis.org/A218309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 2 * Σ_{k=0..n} (3*k+4)^(n-1) * binom(n,k) (n>0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial_perm.h"

/* Exponentiation by squaring for seq_t */
static seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t exp = n - 1;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t base   = (seq_t)3 * (seq_t)k + (seq_t)4;
        seq_t power  = seq_pow_seq(base, exp);
        seq_t bin    = binomial_perm(n, k);
        sum += power * bin;
    }

    return sum * (seq_t)2;
}
