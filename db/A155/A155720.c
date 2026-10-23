/*
 * A155720  Triangle read by rows: let f(n,m)=Product[k + m + 1, {k, 0, n}];
 *          t0(n,m)=f(n, m)/(f(n - m, m)); then t(n,m)=t0(n,m)+t0(n,n-m).
 * https://oeis.org/A155720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A155720:
   t0(n,m) = ∏_{k=n+2}^{n+m+1} k   (product of m consecutive integers,
                                   empty product = 1 when m = 0)
   t(n,m)  = t0(n,m) + t0(n,n-m)
   a(n)    = t(row, column) where row is the largest r with
             r*(r+1)/2 ≤ n and column = n - r*(r+1)/2   (row‑major order) */

seq_t a(idx_t n)
{
    /* Find the row number r such that offset_row(r) ≤ n < offset_row(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {      /* offset_row(r+1) = (r+1)(r+2)/2 */
        r++;
    }
    idx_t offset = r * (r + 1) / 2;           /* offset_row(r)   */
    idx_t m = n - offset;                     /* column index */

    /* t0(r,m) */
    seq_t val1 = 1;
    if (m > 0) {
        idx_t start = r + 2;
        idx_t end   = m + r + 1;
        for (idx_t k = start; k <= end; ++k) {
            val1 *= (seq_t)k;
        }
    }

    /* t0(r, r-m) */
    idx_t m2 = r - m;
    seq_t val2 = 1;
    if (m2 > 0) {
        idx_t start = r + 2;
        idx_t end   = m2 + r + 1;
        for (idx_t k = start; k <= end; ++k) {
            val2 *= (seq_t)k;
        }
    }

    return val1 + val2;
}
