/*
 * A198063  Triangle read by rows (n >= 0, 0 <= k <= n, m = 3); T(n,k) =
 *          Sum{j=0..m} Sum{i=0..m} (-1)^(j+i)*C(i,j)*n^j*k^(m-j).
 * https://oeis.org/A198063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where * r = floor((sqrt(1+8*n)-1)/2), * k = n -
 *            r*(r+1)/2, * T(r,k) = 2*k*k*r - 2*k*r*r + r*r*r. * * This is the
 *            triangle A198063 read by rows
 * Range:     u8 n <= 27, u16 n <= 860, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    seq_t rs = (seq_t)r;                     /* row index as seq_t */
    seq_t baseIdx = rs * (rs + 1) / 2;       /* index of first term in this row */
    seq_t ks = (seq_t)n - baseIdx;           /* column index as seq_t */

    /* Compute T(r,k) = 2*k^2*r - 2*k*r^2 + r^3 */
    seq_t result = 2 * ks * ks * rs
                 - 2 * ks * rs * rs
                 + rs * rs * rs;

    return result;
}
