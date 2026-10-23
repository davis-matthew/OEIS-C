/*
 * A174449  Triangle T(n, k, q) = n!*(n+1)!*q^k/((n-k)!(n-k+1)!) if floor(n/2)
 *          > k-1, otherwise n!*(n+1)!*q^(n-k)/(k!*(k+1)!) for q = 1, read by
 *          rows.
 * https://oeis.org/A174449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattening of the triangle T(n,k,1) where T(r,k) =
 *            Π_{i=0}^{k-1} (r-i)*(r+1-i) with k = min(k', r-k') and k' = n -
 *            r(r+1)/2. r is chosen so that r(r+1)/2 ≤ n < (r+1)(r+2)/2
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 70, u64 n <= 179,
 *            u128 n <= 479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t row_from_index(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 by binary search. */
    idx_t lo = 0;
    idx_t hi = 1;

    /* Grow hi until the inequality fails. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n)
        hi <<= 1;

    lo = hi >> 1; /* lo is now a lower bound */

    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 mid_val = (unsigned __int128)mid * (mid + 1) / 2;
        if (mid_val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Determine the row r and the column offset within that row. */
    idx_t r = row_from_index(n);
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - offset;              /* column index (0‑based)          */
    idx_t k1 = (k <= r - k) ? k : r - k;  /* use symmetry to keep k small */

    seq_t prod = 1;
    for (idx_t i = 0; i < k1; ++i) {
        prod *= (seq_t)(r - i);
        prod *= (seq_t)(r + 1 - i);
    }
    return prod;
}
