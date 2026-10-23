/*
 * A117184  Riordan array ((1+x)c(x^2)/sqrt(1-4x^2),xc(x^2)), c(x) the g.f. of
 *          A000108.
 * https://oeis.org/A117184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row index with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, k = n – r(r+1)/2, and T(r,k) =
 *            binom(r+1,(r+k)/2+1) if r–k is even, = binom(r, (r+k+1)/2)
 *            otherwise
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = 1;
    /* increase high until the triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;

    /* binary search on [low, high] */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    unsigned __int128 tri_r = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri_r;          /* position inside the row */

    /* Compute T(r,k) using binomial coefficients */
    if (((r - k) & 1U) == 0) {           /* r - k even */
        idx_t choose = (r + k) / 2 + 1;
        return binom(r + 1, choose);
    } else {                              /* r - k odd */
        idx_t choose = (r + k + 1) / 2;
        return binom(r, choose);
    }
}
