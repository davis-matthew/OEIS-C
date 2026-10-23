/*
 * A186414  a(n) = binomial(2n,n)^3/(n+1)^2.
 * https://oeis.org/A186414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(2*n,n)^3/(n+1)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* C(2n,n) */
    seq_t t = binomial(2 * n, n);

    /* Catalan number: C(2n,n)/(n+1) */
    seq_t cat = t / ((seq_t)n + 1);

    /* t^2/(n+1) = t * cat */
    seq_t t2div = t * cat;

    /* a(n) = (t^2/(n+1)) * cat = t^3/(n+1)^2 */
    return t2div * cat;
}
