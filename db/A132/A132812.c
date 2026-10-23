/*
 * A132812  Triangle read by rows, n>=1, 1<=k<=n, T(n,k) =
 *          k*binomial(n,k)^2/(n-k+1).
 * https://oeis.org/A132812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry of the triangle T(n,k) read by rows. Row
 *            index m is the unique integer with m(m+1)/2 >= n > (m-1)m/2.
 *            Column index k = n - (m-1)m/2. T(m,k) = binomial(m,k) *
 *            binomial(m,k-1)
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 179, u64 n <= 612,
 *            u128 n <= 2308, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row m by binary search on m(m+1)/2 >= n.
       Use 128-bit arithmetic to avoid overflow. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod >= (unsigned __int128)2 * n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;

    /* Compute the previous cumulative count of terms
       to find the column index k. */
    idx_t prev = ((m - 1) * m) / 2;
    idx_t k = n - prev;          /* 1 <= k <= m */

    /* T(m,k) = binomial(m,k) * binomial(m, k-1) */
    seq_t b1 = binomial(m, k);
    seq_t b2 = binomial(m, k - 1);
    return b1 * b2;
}
