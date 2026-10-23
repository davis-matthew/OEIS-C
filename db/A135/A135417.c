/*
 * A135417  Number of mountain numbers (see A134941) with n digits.
 * https://oeis.org/A135417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(1) = 1 a(2) = 0 a(n) = Σ_{a=1}^{8} C( 2*(a-1) , n-3 ) for n ≥
 *            3 where C(n,k) is 0 if k<0 or k>n. The sum is zero for n>17
 *            because the largest binomial index is 2*(8-1)=14. Hence a(n)=0
 *            for n>17
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the small offsets */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;
    if (n > (idx_t)17) return (seq_t)0;

    /* For n ≥ 3 we sum the binomial terms */
    idx_t k = n - 3;           /* shift the index */
    seq_t sum = (seq_t)0;

    for (idx_t a = 1; a <= 8; ++a)
    {
        idx_t r = 2ULL * (a - 1);      /* r = 2*(a-1)  */
        if (k <= r)                    /* binom(r,k) is 0 otherwise */
            sum += binom(r, k);
    }
    return sum;
}
