/*
 * A190730  Let b(n,0) = n and b(n,k) = 2*b(n,k-1) + 1 for k > 0. Then a(n) =
 *          b(n,1) + b(n,2) + ... + b(n,n).
 * https://oeis.org/A190730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^(n+1) - 3*n - 2
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t sn1 = sn + 1;                   /* n+1 */
    seq_t two_pow = pow2(n + 1);          /* 2^(n+1) */

    /* Compute (n+1)*2^(n+1) */
    seq_t part = sn1 * two_pow;

    /* Subtract 3*n + 2 */
    seq_t sub = (seq_t)3 * sn + 2;

    return part - sub;
}
