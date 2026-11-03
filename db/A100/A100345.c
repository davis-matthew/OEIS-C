/*
 * A100345  Triangle read by rows: T(n,k) = n*(n+k), 0 <= k <= n.
 * https://oeis.org/A100345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r * (r + p) where r = row_of_index(n) (the unique r with
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2) and p = n - r(r+1)/2 (position
 *            within the row)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t row_of_index(idx_t n)
{
    /* Binary search for the largest r with r*(r+1)/2 <= n. */
    idx_t low = 0, high = n, mid, tri;
    idx_t r = 0;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        tri = mid * (mid + 1) / 2;
        if (tri <= n) { r = mid; low = mid + 1; }
        else           high = mid - 1;
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t r = row_of_index(n);                    /* compute row number */
    seq_t r_seq = (seq_t)r;
    seq_t n_seq = (seq_t)n;
    seq_t t = r_seq * (r_seq + 1) / 2;             /* start index of row r */
    seq_t p = n_seq - t;                           /* offset in the row */
    return r_seq * (r_seq + p);                    /* T(r, p) = r*(r+p) */
}
