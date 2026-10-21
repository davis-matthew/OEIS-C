/*
 * A099045  a(n) = (3*0^n + 4^n*binomial(2*n,n))/4.
 * https://oeis.org/A099045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*0^n + 4^n*binomial(2*n,n))/4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;               /* 3*0^0 + 4^0*1 = 4, 4/4 = 1 */

    /* Compute binomial(2n, n) */
    idx_t two_n = n + n;
    seq_t bin = binomial(two_n, n);

    /* Compute 4^(n-1) */
    seq_t pow4 = pow_int(4, n - 1);

    /* a(n) = bin * 4^(n-1) */
    return bin * pow4;
}
