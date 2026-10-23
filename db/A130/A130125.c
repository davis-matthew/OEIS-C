/*
 * A130125  Triangle defined by A128174 * A130123, read by rows.
 * https://oeis.org/A130125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^k if (row(n)-k) is even, else 0, where row(n) is the
 *            unique integer r with r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 5150, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
static idx_t find_row(idx_t n)
{
    const idx_t HIGH_EST = (idx_t)4294967296ULL; /* 2^32, large enough for all practical n */
    idx_t lo = 0, hi = HIGH_EST;
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    __int128 tri128 = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri128;     /* column index within the row */
    if (((r - k) & 1) != 0)          /* if row-k is odd, entry is 0 */
        return 0;
    return pow2(k);                  /* otherwise 2^k */
}
