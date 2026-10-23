/*
 * A163652  Triangle read by rows where T(n,m)=2*m*n + m + n + 6.
 * https://oeis.org/A163652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2*m*r + m + r + 6, * where (r,m) are the unique row r ≥ 1
 *            and column * 1 ≤ m ≤ r such that the n‑th element of the
 *            triangle * read by rows is the m‑th element of row r. * The
 *            triangle has entries T(r,m) = 2*r*m + r + m + 6
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the row index r satisfying
 * r*(r+1)/2 ≥ n  and (r-1)*r/2 < n.
 * Uses binary search with 128‑bit arithmetic to avoid overflow. */
static idx_t row_of(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* Find an upper bound. */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;

    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column m for the n‑th term. */
    idx_t r = row_of(n);

    /* Number of terms before row r. */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t m    = n - prev;            /* column within the row */

    /* Compute the triangle entry using seq_t arithmetic. */
    seq_t R = (seq_t)r;
    seq_t M = (seq_t)m;
    return 2 * (M * R) + M + R + 6;
}
