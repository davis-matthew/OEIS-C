/*
 * A125230  Triangle T(n,k) (0<=k<=n) read by rows in which column k contains
 *          the binomial transform of the sequence of k 0's, (k+1) 1's,
 *          followed by 0's.
 * https://oeis.org/A125230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=k..min(2*k,r)} C(r,j) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 37, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* integer square root for unsigned 64‑bit values */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (x >> 1) + 1;
    while (low < high)
    {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* compute row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t t = (n << 3) + 1;               /* 8*n + 1 */
    idx_t r = (_isqrt_u64(t) - 1) >> 1;
    idx_t row_start = (r * (r + 1)) >> 1; /* r*(r+1)/2 */
    idx_t k = n - row_start;
    idx_t upper = (k << 1 <= r) ? (k << 1) : r;
    seq_t sum = 0;
    for (idx_t j = k; j <= upper; ++j)
        sum += binom(r, j);
    return sum;
}
