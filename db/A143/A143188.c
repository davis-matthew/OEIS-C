/*
 * A143188  Triangle read by rows: T(n, k) = f(k) for 1 <= k <= floor(n/2),
 *          T(n, k) = f(n-k) for floor(n/2) < k <= n-1, with T(n, 0) = 1, T(n,
 *          n) = 1, and f(k) = (1/2)*(3-(-1)^k)*k*binomial(n, k).
 * https://oeis.org/A143188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where - r = floor((sqrt(8*n+1)-1)/2) (row index) -
 *            k = n - r*(r+1)/2 (column index in the row) For T(n,k) the
 *            definition is: T(n,0)=T(n,n)=1; for 1 <= k <= floor(n/2): T(n,k)
 *            = f(k) for floor(n/2) < k <= n-1: T(n,k) = f(n-k) with f(k) =
 *            (1/2)*(3-(-1)^k) * k * binomial(n,k)
 * Range:     u8 n <= 38, u16 n <= 126, u32 n <= 479, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r and k by iterating over rows until the cumulative count
       exceeds n.  This is O(sqrt(n)) which is trivial for n <= 10000. */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + r + 1 <= n)
    {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;                 /* column index in row r */

    /* boundary values */
    if (k == 0 || k == r) return (seq_t)1;

    /* use symmetry to bring the argument to [1, floor(r/2)] */
    idx_t m = (k <= r - k) ? k : r - k;

    /* compute binomial coefficient */
    seq_t bin = binomial(r, m);           /* robust, arbitrary precision */

    /* f(m) = m * bin * factor  */
    seq_t val = bin * (seq_t)m;
    if (m & (idx_t)1)                     /* factor = 2 when m is odd */
        val = val * (seq_t)2;

    return val;
}
