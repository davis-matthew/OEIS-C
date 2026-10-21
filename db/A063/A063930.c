/*
 * A063930  Radius of B-excircle of Pythagorean triangle with a=(n+1)^2-m^2,
 *          b=2*(n+1)*m and c=(n+1)^2+m^2.
 * https://oeis.org/A063930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = m(n+m+1) where (s,m) are the row and column indices *
 *            obtained by anti‑diagonal enumeration of the triangle with
 *            a(s,m)=m(s+m+1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* offset starts at 1 */

    /* Find the row index s such that s*(s+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                      /* row index, s >= 1 */

    /* Compute the column index m */
    idx_t prev_sum = (idx_t)(((__int128)(s - 1) * s) / 2);
    idx_t m = n - prev_sum;             /* 1 <= m <= s */

    /* Return m * (s + m + 1) */
    seq_t M = (seq_t)m;
    seq_t S = (seq_t)s;
    return M * (S + M + 1);
}
