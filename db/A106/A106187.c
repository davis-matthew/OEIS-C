/*
 * A106187  Sequence array for central binomial numbers A000984.
 * https://oeis.org/A106187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial( 2*(r-k), r-k ) where r is the unique integer
 *            with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n – r(r+1)/2. This is
 *            the entry in the triangular array T(n,k)=binomial(2*(n-k),n-k)
 *            listing the central binomial numbers row by row
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0, high = 1;
    /* exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* binary search between r and high */
    while (r + 1 < high) {
        idx_t mid = r + (high - r) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            r = mid;
        else
            high = mid;
    }
    /* index within the row */
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;
    /* the binomial index m = r - k */
    idx_t m = r - k;
    return binom(2 * m, m);
}
