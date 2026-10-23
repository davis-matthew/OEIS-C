/*
 * A008464  a(n) = 2^(2n+3) - 2^n*(n+3).
 * https://oeis.org/A008464
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+3) – 2^n*(n+3) Initial values: a(-1)=1, a(0)=5,
 *            a(1)=24, a(2)=108,
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* offset -1: only needed value for n = -1 */
    if (n == -1)
        return (seq_t)1;

    /* for n < -1 the function is undefined; return 0 */
    if (n < -1)
        return (seq_t)0;

    /* Compute 2^(2n+3) */
    idx_t exp1 = 2 * n + 3;
    seq_t term1 = pow2(exp1);

    /* Compute 2^n * (n+3) */
    seq_t pow2_n = pow2(n);
    seq_t term2 = pow2_n * ((seq_t)(n + 3));

    return term1 - term2;
}
