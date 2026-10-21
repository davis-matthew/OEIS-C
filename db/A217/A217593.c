/*
 * A217593  Square array T, read by antidiagonals: T(n,k) = 0 if n-k >=1 or if
 *          k-n >= 9, T(0,k) = 1 for k = 0..8, T(n,k) = T(n-1,k) + T(n,k-1).
 * https://oeis.org/A217593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value of the square array T read by antidiagonals *
 *            where * T(n,k) = 0 if n-k >= 1 (n > k) * 0 if k-n >= 9 (k > n+8)
 *            * T(0,k) = 1 for k = 0..8 * T(n,k) = T(n-1,k) + T(n,k-1) for all
 *            other n,k ≥ 0 * * The sequence a(n) is obtained by traversing T
 *            by antidiagonals (i+j = const, * i incr...
 * Range:     u8 n <= 81, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal index s such that
     *   s*(s+1)/2 ≤ n < (s+1)*(s+2)/2.
     * Use long double for the square root and correct any rounding errors. */
    long double nd = (long double)n;
    long double sf = (sqrtl(8.0L * nd + 1.0L) - 1.0L) / 2.0L;
    idx_t s = (idx_t)sf;

    /* Correct s if the floating rounding was off */
    while ((unsigned __int128)s * (s + 1) / 2 > n) --s;
    while ((unsigned __int128)(s + 1) * (s + 2) / 2 <= n) ++s;

    idx_t pref = (idx_t)((unsigned __int128)s * (s + 1) / 2); /* terms before diag s */
    idx_t i = n - pref;                /* row index in diagonal s */
    idx_t j = s - i;                   /* column index in diagonal s */

    /* Values outside the band are zero */
    if (i > j || j - i >= 9) return 0;

    /* DP arrays for two consecutive rows; we store values for
     * delta = k - n   (0 ≤ delta ≤ 8) */
    seq_t prev[9];
    seq_t curr[9];

    /* Base row: T(0,k) = 1 for k = 0..8 */
    for (int d = 0; d <= 8; ++d) prev[d] = 1;

    /* If we're on the base row, the answer is already known */
    if (i == 0)
        return prev[(int)(j - i)];

    /* Compute rows 1 … i */
    for (idx_t row = 1; row <= i; ++row) {
        for (int d = 0; d <= 8; ++d) {
            seq_t up   = (d <= 7) ? prev[d + 1] : 0; /* T(row-1, k) where k - (row-1) = d+1 */
            seq_t left = (d > 0) ? curr[d - 1] : 0;  /* T(row,   k-1)  where (k-1) - row = d-1 */
            curr[d] = up + left;
        }
        /* Move current row to prev for next iteration */
        for (int d = 0; d <= 8; ++d) prev[d] = curr[d];
    }

    /* After reaching row i, return the required entry. */
    return prev[(int)(j - i)];
}
