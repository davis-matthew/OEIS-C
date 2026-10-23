/*
 * A193722  Triangular array: the fusion of (x+1)^n and (x+2)^n; see Comments
 *          for the definition of fusion.
 * https://oeis.org/A193722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if k = 0; otherwise * T(r,k) = 3^(k-1) * (
 *            binomial(r-1,k) + 2*binomial(r,k) ), * where r is the row index
 *            satisfying r(r+1)/2 <= n < (r+1)(r+2)/2 * and k = n - r(r+1)/2.
 *            This implements the triangular array * given by A193722
 * Range:     u8 n <= 18, u16 n <= 60, u32 n <= 181, u64 n <= 617,
 *            u128 n <= 2257, big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* special case: the first term of every row */
    if (n == 0)
        return (seq_t)1;

    /* find row r via binary search; r satisfies r*(r+1)/2 <= n */
    idx_t lo = 0, hi = 1;
    while (((__int128)hi * (hi + 1)) / 2 <= n)
    {
        hi <<= 1;
        if (hi > n + 1)
        {
            hi = n + 1;
            break;
        }
    }
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;

    /* compute column k */
    idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - offset;

    /* top of each row equals 1 */
    if (k == 0)
        return (seq_t)1;

    /* compute binomial coefficients and the power of 3 */
    seq_t bin1 = binomial(r - 1, k);        /* C(r-1, k) */
    seq_t bin2 = binomial(r, k);            /* C(r, k)   */
    seq_t sum  = bin1 + bin2 + bin2;         /* bin1 + 2*bin2 */
    seq_t pow3 = pow_uint(3, k - 1);

    return pow3 * sum;
}
