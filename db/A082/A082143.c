/*
 * A082143  First subdiagonal of number array A082137.
 * https://oeis.org/A082143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-1) * binomial(2*n+1, n) for n>0 * a(0) = 1 * (Second
 *            term 0^n/2 is zero for n>0, and 0^0/2 contributes 1/2 when n=0,
 *            * giving the correct integer result.)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p = pow2(n - 1);          /* 2^(n-1)  */
    seq_t b = binomial(2 * n + 1, n);/* C(2n+1,n)*/
    return p * b;
}
