/*
 * A096038  Triangle T(n,m) = (3*n^2-3*m^2+5*m-4+n)/2 read by rows.
 * https://oeis.org/A096038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   the sequence is the triangle * T(n,m) = (3*n^2 - 3*m^2 + 5*m - 4
 *            + n)/2 read by rows. * a(idx) is the idx‑th element of the
 *            triangle, i.e. T(row,col) where * the linear index idx starts
 *            from 1
 * Range:     u8 n <= 45, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t floor_sqrt_idx(idx_t v)
{
    /* binary search, safe for v < 2^64 */
    idx_t lo = 0, hi = v;
    idx_t res = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * mid <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t idx)
{
    /* find the row n such that n*(n+1)/2 >= idx */
    idx_t n = (floor_sqrt_idx(8 * idx + 1) - 1) / 2;
    while ((n * (n + 1)) / 2 < idx) ++n;
    while ((n - 1) * n / 2 >= idx) --n;

    /* column m = idx - (n-1)*n/2 */
    idx_t m = idx - (n - 1) * n / 2;

    seq_t sn = (seq_t)n;
    seq_t sm = (seq_t)m;

    seq_t num = 3 * sn * sn - 3 * sm * sm + 5 * sm - 4 + sn;
    return num / 2;
}
