/*
 * A107867  Triangle, read by rows, where T(n,k) =
 *          C(n*(n-1)/2-k*(k-1)/2+n-k+1,n-k).
 * https://oeis.org/A107867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(r,k)=C(r*(r-1)/2 - k*(k-1)/2 + r - k
 *            + 1, r - k), row is the largest integer r with r*(r+1)/2 ≤ n,
 *            and col = n - r*(r+1)/2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_seq.h"

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
static inline idx_t
row_of_n(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Expand hi until hi*(hi+1)/2 > n.  Use __int128 to avoid overflow. */
    while (((__int128)hi * (hi + 1) / 2) <= n && hi <= (idx_t)(-1) >> 1)
        hi <<= 1;
    /* Binary search in [lo, hi]. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (((__int128)mid * (mid + 1) / 2) <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    const idx_t r = row_of_n(n);
    const idx_t offset = (idx_t)(((__int128)r * (r + 1)) / 2);
    const idx_t k = n - offset;

    const __int128 row_term = (__int128)r * (r - 1) / 2;
    const __int128 col_term = (__int128)k * (k - 1) / 2;
    const __int128 start128 = row_term - col_term + (r - k) + 1;
    const idx_t start = (idx_t)start128;
    const idx_t choose = r - k;

    return binom_seq(start, choose);
}
