/*
 * A155550  Triangle read by rows where T(m,n)=2*m*n + m + n - 6.
 * https://oeis.org/A155550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 2*m*k + m + k - 6, * where m is the smallest integer such
 *            that m*(m+1)/2 ≥ n, * and k = n - m*(m-1)/2. This flattens the
 *            triangle T(m,k) * with T(m,k)=2*m*k + m + k - 6 read row‑by‑row
 * Range:     i8 n <= 35, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* below offset – undefined, return 0 */

    /* find m such that m*(m+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t m = low;

    /* position k inside row m */
    idx_t before = (idx_t)(((__int128)m * (m - 1)) / 2);
    idx_t k = n - before;

    /* compute the triangle value */
    seq_t val = (seq_t)2 * (seq_t)m * (seq_t)k + (seq_t)m + (seq_t)k - 6;
    return val;
}
