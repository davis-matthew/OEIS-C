/*
 * A204006  Symmetric matrix based on f(i,j) = min{2i+j-2,i+2j-2}, by
 *          antidiagonals.
 * https://oeis.org/A204006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = min(2*i + j - 2, i + 2*j - 2), where (i,j) are the *
 *            antidiagonal coordinates of the nth element: * - let m be the
 *            smallest integer with m*(m+1)/2 ≥ n * - then i = t, j = (m+1) -
 *            t, where t = n - (m-1)*m/2 * (1 ≤ t ≤ m)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal length m such that m*(m+1)/2 ≥ n */
    idx_t lo = 1;
    idx_t hi = n;                /* upper bound for m */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t m = lo;                /* length of the current diagonal */

    /* Number of elements before this diagonal */
    __int128 prev = (__int128)(m - 1) * m / 2;
    idx_t t = (idx_t)(n - prev); /* position inside the diagonal (1..m) */

    /* Coordinates (i,j) */
    idx_t i = t;
    idx_t j = (m + 1) - t;

    /* Compute f(i,j) using 128‑bit arithmetic to avoid overflow */
    __int128 v1 = (__int128)2 * i + j - 2;
    __int128 v2 = (__int128)i + (__int128)2 * j - 2;

    return (v1 < v2) ? (seq_t)v1 : (seq_t)v2;
}
