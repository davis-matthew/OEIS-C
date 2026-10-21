/*
 * A087540  Let A(n) be the matrix in the group GL(n,2) such that for 1 <= i,
 *          j <= n: A[i,j] = 1 if i+j = n+1 A[i,j] = 0 if i+j != n+1. a(n) is
 *          the size of the centralizer of A(n) in GL(n,2).
 * https://oeis.org/A087540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For even n = 2m: a(n) = 2^(m^2) * |GL(m,2)| For odd n = 2m+1:
 *            a(n) = 2^(m^2+2m)* |GL(m,2)| where |GL(m,2)| = ∏_{k=0}^{m-1}
 *            (2^m – 2^k)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"   /* for pow_int(seq_t base, idx_t exp)   */
#include "../utils/big_pow.h"   /* for big_pow(seq_t base, seq_t exp)   */

seq_t a(idx_t n)
{
    /* m = floor(n/2) */
    idx_t m = n >> 1;

    /* extra exponent part (only if n is odd) */
    idx_t e = (n & 1ULL) ? (m << 1) : 0ULL;

    /* total exponent of 2 */
    idx_t shift = m * m + e;

    /* compute |GL(m,2)|  */
    seq_t result = (seq_t)1;
    seq_t base   = pow_int((seq_t)2, m);   /* 2^m */

    seq_t sub = (seq_t)1;                  /* 2^0 */
    for (idx_t k = 0; k < m; ++k)
    {
        result *= (base - sub);
        sub <<= 1;                         /* next power of 2 */
    }

    /* multiply by 2^shift  */
    if (shift < (sizeof(seq_t) * 8ULL))
    {
        result <<= shift;
    }
    else
    {
        result *= big_pow((seq_t)2, (seq_t)shift);
    }

    return result;
}
