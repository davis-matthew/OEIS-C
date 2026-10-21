/*
 * A072100  Column 2 of the array m(i,1)=m(1,j)=1 m(i,j)=m(i-1,j-1)+m(i-1,j+1)
 *          (a(n)=m(n,2)).
 * https://oeis.org/A072100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-2} binomial(k, floor(k/2))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial helper is provided by the build environment. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;        /* a(1) = 1 */

    seq_t sum = (seq_t)1;        /* the leading 1 in the formula */

    for (idx_t k = 0; k <= n-2; ++k)
    {
        /* floor(k/2) fits in idx_t because k <= n-2 <= 9998 for n<=10000 */
        sum += binom(k, k/2);
    }

    return sum;
}
