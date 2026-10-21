/*
 * A081130  Square array of binomial transforms of (0,0,1,0,0,0,...), read by
 *          antidiagonals.
 * https://oeis.org/A081130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(n',k'), where T(n,k)=k^(n-2)*C(n,2) (with T(n,0)=0) and
 *            (n',k') is the index on the antidiagonal that contains the term
 *            a(n). This implements the OEIS definition of A081130
 * Range:     u8 n <= 40, u16 n <= 84, u32 n <= 200, u64 n <= 454,
 *            u128 n <= 1207, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Find the largest integer d such that d*(d+1)/2 <= n.
   Uses a binary search; all arithmetic that can overflow is performed
   in __int128.  */
static idx_t diag_of(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = 1;
    /* grow hi until it is > n */
    while (((unsigned __int128)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;
    /* binary search between lo and hi-1 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine which diagonal the term lies on. */
    idx_t d = diag_of(n);

    /* Number of terms in all previous diagonals. */
    idx_t prev = ((unsigned __int128)d * (d + 1)) / 2;

    /* Index inside the diagonal and column index. */
    idx_t i = n - prev;      /* row (n in T(n,k))   */
    idx_t j = d - i;         /* column (k in T(n,k)) */

    /* For rows 0 and 1, or for column 0, T = 0. */
    if (j == 0 || i < 2)
        return 0;

    /* Compute C(i,2) = i*(i-1)/2. */
    seq_t bin = (seq_t)i * (seq_t)(i - 1) / 2;

    /* Compute j^(i-2).  big_pow works with seq_t arguments. */
    seq_t pwr = big_pow((seq_t)j, (seq_t)(i - 2));

    return pwr * bin;
}
