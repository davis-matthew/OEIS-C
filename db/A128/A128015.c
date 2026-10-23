/*
 * A128015  Binomial coefficients C(2n+1,n) repeated.
 * https://oeis.org/A128015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+1, n/2) for even n; C(n, (n-1)/2) for odd n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)          /* n is even */
        return binom(n + 1, n >> 1);
    else                       /* n is odd  */
        return binom(n, (n - 1) >> 1);
}
