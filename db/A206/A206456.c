/*
 * A206456  Number of 0..n arrays of length n+2 avoiding the consecutive
 *          pattern 0..n.
 * https://oeis.org/A206456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^(n+2) - 2*(n+1) // from the OEIS definition This is
 *            equivalent to a(n) = Sum_{i=0..1} (-1)^i*(n+1)^((n+2)-(n+1)*i) *
 *            binomial((n+2)-n*i,i) which simplifies to the given closed form
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety, return 0 for n < 1. */
    if (n < 1)
        return 0;

    seq_t base = (seq_t)(n + 1);
    seq_t exp  = (seq_t)(n + 2);

    /* Compute (n+1)^(n+2) using the provided big_pow routine. */
    seq_t pow = big_pow(base, exp);

    /* Subtract 2*(n+1).  The intermediate multiplication fits in seq_t
       for all n values that the build’s seq_t type can represent. */
    seq_t sub = (seq_t)2 * base;

    return pow - sub;
}
