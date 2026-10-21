/*
 * A224436  The hyper-Wiener index of the polyphenylene dendrimer G[n] defined
 *          pictorially in the N. E. Arif et al. reference.
 * https://oeis.org/A224436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 407284 * + (-2453054 + 211431*n - 103545*n^2) * 2^n * +
 *            (2049492 - 1303920*n + 608400*n^2) * 4^n * Initial value: a(0) =
 *            3722 * * The calculation is performed with 64‑bit signed
 *            intermediates * for the quadratic coefficients to avoid negative
 *            values * being cast to unsigned seq_t. Powers o...
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 18, u128 n <= 48,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* special case for n = 0 */
    if (n == 0)
        return 3722;

    /* 64‑bit signed intermediates for the polynomials */
    int64_t n64 = (int64_t)n;
    int64_t nn  = n64 * n64;              /* n^2 */
    int64_t coeff1 = -2453054 + 211431 * n64 - 103545 * nn; /* negative */
    int64_t coeff2 =  2049492 - 1303920 * n64 + 608400 * nn; /* positive */

    /* Constants become positive for the multiplication */
    int64_t A = -coeff1;           /* |coeff1| */
    seq_t   p2   = pow2(n);        /* 2^n   */
    seq_t   p4   = pow4(n);        /* 4^n   */

    seq_t termNeg = (seq_t)A * p2;          /* |coeff1| * 2^n */
    seq_t termPos = (seq_t)coeff2 * p4;     /* coeff2   * 4^n */

    seq_t result = 407284 + termPos - termNeg;
    return result;
}
