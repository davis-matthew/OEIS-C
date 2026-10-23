/*
 * A185911  Weight array of A185910, by antidiagonals.
 * https://oeis.org/A185911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) are the row and column obtained from
 *            the antidiagonal (NW to SE) traversal of the triangular array T
 *            defined by T(1,k) = 1 (k ≥ 1) T(n,1) = 2*n-1 (n ≥ 1) T(n,k) = 0
 *            otherwise. Then a(n) is 1 if i==1, 2*i-1 if j==1, and 0
 *            otherwise
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety: offset is 1 */

    /* Find the smallest d such that d*(d+1)/2 ≥ n  (the diagonal index). */
    idx_t low = 1, high = n;              /* n ≥ 1 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                         /* diagonal number */

    /* Position within diagonal d (1‑based). */
    __int128 prev_tri = (__int128)(d - 1) * d / 2;
    idx_t pos = n - (idx_t)prev_tri;        /* 1 ≤ pos ≤ d */

    idx_t i = pos;                          /* row index   */
    idx_t j = d - pos + 1;                  /* column index */

    if (i == 1)
        return (seq_t)1;
    else if (j == 1) {
        seq_t val = (seq_t)i * (seq_t)2;
        val -= (seq_t)1;
        return val;
    } else
        return (seq_t)0;
}
