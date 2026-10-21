/*
 * A215546  Number of standard Young tableaux of shape [6n,6].
 * https://oeis.org/A215546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n-5)*(6*n+5)*(3*n+2)*(2*n+1)*(3*n+1)*(n+1))/10 for
 *            n>0, a(0)=0
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 19, u64 n <= 810, u128 n <= 1000,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

static inline seq_t reduce_denom(seq_t *num, seq_t *denom)
{
    seq_t g = seq_gcd(*num, *denom);
    *num /= g;
    *denom /= g;
    return g;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;
    seq_t denom  = 10;

    seq_t t;

    t = ((seq_t)6 * (seq_t)n) - 5;   reduce_denom(&t, &denom); result *= t;
    t = ((seq_t)6 * (seq_t)n) + 5;   reduce_denom(&t, &denom); result *= t;
    t = ((seq_t)3 * (seq_t)n) + 2;   reduce_denom(&t, &denom); result *= t;
    t = ((seq_t)2 * (seq_t)n) + 1;   reduce_denom(&t, &denom); result *= t;
    t = ((seq_t)3 * (seq_t)n) + 1;   reduce_denom(&t, &denom); result *= t;
    t = (seq_t)n + 1;                reduce_denom(&t, &denom); result *= t;

    if (denom > 1) result /= denom;   /* should never happen */

    return result;
}
