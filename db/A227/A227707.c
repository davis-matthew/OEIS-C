/*
 * A227707  The terminal Wiener index of the dendrimer D_n defined pictorially
 *          in Fig. 1 of the Heydari et al. reference.
 * https://oeis.org/A227707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 9*4^n*(n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n using the provided utilities */
    seq_t two_pow = pow2(n);   /* 2^n */
    seq_t four_pow = pow4(n);  /* 4^n */

    /* first part: 3*2^n */
    seq_t term1 = two_pow * (seq_t)3;

    /* second part: 9*4^n*(n+1) */
    seq_t term2 = four_pow * (seq_t)9;
    term2 *= ((seq_t)n + 1);

    /* sum both parts */
    return term1 + term2;
}
