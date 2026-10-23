/*
 * A115473  Number of monic irreducible polynomials of degree 2 in
 *          GF(2)[x1,...,xn].
 * https://oeis.org/A115473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n+1} * (2^{n(n+1)/2} - 1) - (2^{n+1} - 2) * (2^{n+1} -
 *            1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, value for n=0 is undefined */

    /* compute powers of two as shifts: (seq_t)1 << exp */
    idx_t exp_n1 = n + 1;               /* exponent n+1 */
    seq_t pow_n1 = ((seq_t)1) << exp_n1;/* 2^(n+1) */

    idx_t exp_M = n * (n + 1) / 2;       /* exponent M = n(n+1)/2 */
    seq_t pow_M = ((seq_t)1) << exp_M;   /* 2^M */

    /* total number of monic quadratics */
    seq_t total = pow_n1 * (pow_M - 1);

    /* number of monic linear polynomials: N1 = 2^(n+1) - 2 */
    seq_t N1 = pow_n1 - 2;

    /* number of reducible monic quadratics (unordered pairs of linear factors) */
    seq_t reducible = N1 * (N1 + 1) / 2;

    return total - reducible;
}
