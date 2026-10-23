/*
 * A163676  Triangle T(n,m) = 4mn + 2m + 2n - 1 read by rows.
 * https://oeis.org/A163676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,m) with r = ceil((sqrt(8*n+1)-1)/2), m = n - r(r-1)/2
 *            and T(n,m) = 4*n*m + 2*m + 2*n - 1
 * Range:     u8 n <= 35, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* return 0 for indices below the offset */
    if (n < 1) return (seq_t)0;

    /* find row r such that r(r+1)/2 >= n  */
    idx_t low = 1, high = 1;
    while (((high) * (high + 1)) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid) * (mid + 1)) / 2 >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;

    /* column index within the row (1‑based) */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t m = n - prev_tri;

    /* compute the value using seq_t arithmetic */
    seq_t sr = (seq_t)r;
    seq_t sm = (seq_t)m;
    return 4 * sr * sm + 2 * sm + 2 * sr - 1;
}
