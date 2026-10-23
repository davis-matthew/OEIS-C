/*
 * A115474  Number of monic irreducible polynomials of degree 3 in
 *          GF(2)[x1,...,xn].
 * https://oeis.org/A115474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of monic irreducible polynomials of degree 3 in
 *            GF(2)[x1,…,xn]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binom, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Binomial coefficients */
    seq_t c_n3_3 = binom(n + 3, 3);
    seq_t c_n2_2 = binom(n + 2, 2);

    /* Powers of two with potentially large exponents */
    seq_t pow1   = pow_int((seq_t)2, (idx_t)c_n3_3);      /* 2^(C(n+3,3))      */
    seq_t pow2   = pow_int((seq_t)2, (idx_t)c_n2_2);      /* 2^(C(n+2,2))      */
    seq_t pow_n1 = pow_int((seq_t)2, n + 1);             /* 2^(n+1)           */

    /* Exact‑degree counts */
    seq_t N3 = pow1 - pow2;              /* monic polynomials of total degree 3 */
    seq_t N2 = pow2 - pow_n1;            /* monic polynomials of total degree 2 */
    seq_t L  = pow_n1 - ((seq_t)2);     /* number of monic linear polynomials */

    /* Binomials of L (up to 3) */
    seq_t C_L_2 = (L * (L - ((seq_t)1))) / ((seq_t)2);
    seq_t C_L_3 = (L * (L - ((seq_t)1)) * (L - ((seq_t)2))) / ((seq_t)6);

    /* Inclusion–exclusion for cubics that are reducible */
    seq_t reducible = (L * N2) - (C_L_2 * L) + C_L_3;

    /* Irreducible monic degree‑3 polynomials */
    seq_t irreducible = N3 - reducible;

    return irreducible;
}
