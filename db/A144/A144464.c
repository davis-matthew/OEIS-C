/*
 * A144464  Triangle T(n,m) read by rows: T(n,m) = 2^min(m,n-m).
 * https://oeis.org/A144464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^min(m, r-m) * where r is the row index satisfying
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2, * and m = n - r(r+1)/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute the row number r for a given index n.
 * Uses only integer arithmetic and runs in O(log r) time. */
static idx_t _row_of(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponentially grow high until its triangular number exceeds n */
    while (((unsigned __int128)high * (high + 1) / 2) <= n) {
        if (high == (idx_t)-1) break;  /* avoid overflow */
        high <<= 1;
    }
    /* Binary search in [low, high) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);
    /* Sum of elements before row r: r(r+1)/2 */
    idx_t prev_sum = (idx_t)(( (unsigned __int128)r * (r + 1) ) / 2);
    idx_t m = n - prev_sum;             /* column index within the row (0‑based) */
    idx_t k = (m <= r - m) ? m : r - m; /* exponent for 2 */
    return pow2(k);
}
