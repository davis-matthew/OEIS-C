/*
 * A225419  Triangle read by rows: T(n,k) (0 <= k <= n) = binomial(2*n+2,k).
 * https://oeis.org/A225419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(2*row + 2, k) where n = row*(row+1)/2 + k, 0 ≤ k
 *            ≤ row. The implementation finds row by binary search on the
 *            triangular numbers
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row number r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Exponential search for an upper bound on r. */
    while ( ((unsigned __int128)high * (high + 1)) / 2 <= n )
    {
        low = high;
        if (high > (UINT64_MAX >> 1))
            { high = UINT64_MAX; break; }
        high <<= 1;
    }

    /* Binary search in [low, high] to find the exact row. */
    while (low < high)
    {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 tri = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t row = low;
    idx_t start = (idx_t)((((unsigned __int128)row * (row + 1)) / 2));
    idx_t k = n - start;

    /* Return the binomial coefficient for this row and column. */
    return binomial(2 * row + 2, k);
}
