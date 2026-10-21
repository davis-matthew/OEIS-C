/*
 * A227489  The hyper-Wiener index of the nanostar dendrimer NSC_5C_6[n],
 *          defined pictorially in the Ashrafi et al. and the Rostami et al.
 *          references.
 * https://oeis.org/A227489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 189505 + 2^n * (360*n^2 - 1148*n - 1285024) + 4^n *
 *            (129600*n^2 - 627120*n + 1095500) (Offset: 1)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for n < 1 */

    seq_t sn      = (seq_t)n;             /* convert index to seq_t */
    seq_t two_pow = pow2(n);              /* 2^n in seq_t */

    /* constant term */
    seq_t result = (seq_t)189505;

    /* polynomial multiplied by 2^n */
    seq_t p1 = (seq_t)360 * sn * sn
             - (seq_t)1148 * sn
             - (seq_t)1285024;
    result += two_pow * p1;

    /* quadratic polynomial multiplied by 4^n = (2^n)^2 */
    seq_t p2 = (seq_t)129600 * sn * sn
             - (seq_t)627120 * sn
             + (seq_t)1095500;
    seq_t two_pow_sq = two_pow * two_pow; /* 4^n */
    result += two_pow_sq * p2;

    return result;
}
