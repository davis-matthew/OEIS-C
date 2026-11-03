/*
 * A110449  Triangle read by rows: T(n,k) = n*((2*k+1)*n+1)/2, 0<=k<=n.
 * https://oeis.org/A110449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = row_of_index(n), k = n - r*(r+1)/2,
 *            T(r,k) = r * ((2*k + 1)*r + 1) / 2 (OEIS A110449 – triangle read
 *            by rows)
 * Range:     u8 n <= 32, u16 n <= 899, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Validated: standard, pass, 82 terms through n = 1219 (2026-11-01)
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

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t base = r * (r + 1) / 2;
    idx_t k = n - base;

    seq_t r_s = (seq_t)r;
    seq_t k_s = (seq_t)k;
    seq_t tmp = ((seq_t)2 * k_s + 1) * r_s + 1;
    return r_s * tmp / 2;
}
