/*
 * A152716  Triangle T(n,k) read by rows: T(n,k) = 4^min(k, n-k) =
 *          4^A004197(n,k).
 * https://oeis.org/A152716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 4^min(k, r−k) * where r is the row number such that
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, * and k = n − r*(r+1)/2 is the
 *            column index within that row
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/* Find the row number r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
   Uses 128‑bit arithmetic to avoid overflow for large indices. */
static idx_t _row_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Expand the upper bound until the start of the next row exceeds n. */
    while (((__uint128_t)hi * (hi + 1)) / 2 <= n)
        hi <<= 1;

    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (((__uint128_t)mid * (mid + 1)) / 2 <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the smallest r with start > n, so r = lo – 1. */
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Determine the row and column of position n. */
    idx_t r = _row_of(n);
    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - start;                 /* column index within the row */
    idx_t exp = (k <= r - k) ? k : r - k; /* minimum distance to an edge */

    /* Return 4^exp. */
    return big_pow((seq_t)4, (seq_t)exp);
}
