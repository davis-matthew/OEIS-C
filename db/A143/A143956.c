/*
 * A143956  Triangle read by rows, A000012 * A136521 * A000012; 1<=k<=n.
 * https://oeis.org/A143956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = dim of the triangle read by rows for OEIS A143956
 *            Sequence definition: the triangle read by rows of the product
 *            A000012 * A136521 * A000012; 1<=k<=n. Row r has r elements:
 *            a_{r,1}=2r-1, and a_{r,k}=2(r-k+1) for k>=2. Thus a(n) is the
 *            (n-th) element of this triangle
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    while (((r * (r + 1)) / 2) < n) {
        ++r;
    }

    idx_t prev = ((r - 1) * r) / 2; /* elements before row r */
    idx_t k = n - prev;             /* 1-based column within the row */

    if (k == 1) {
        return (seq_t)((seq_t)2 * r - (seq_t)1);
    } else {
        return (seq_t)((seq_t)2 * (r - k + (seq_t)1));
    }
}
