/*
 * A157153  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) - m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 2, read by rows.
 * https://oeis.org/A157153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = row‑major read of triangle T(n,k,2) defined by: T(n,k,2)
 *            = (2*(n-k)+1)*T(n-1,k-1,2) + (2*k+1)*T(n-1,k,2) -
 *            2*k*(n-k)*T(n-2,k-1,2) with T(n,0,2)=T(n,n,2)=1
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 81, u64 n <= 179,
 *            u128 n <= 479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any 0 ≤ n, O(sqrt(n)) time, O(1) memory. */
seq_t a(idx_t n)
{
    /* Maximum row needed for n ≤ 10000: r ~ 141.   */
    /* Use a safe fixed upper bound.                 */
    const idx_t MAX_ROW = 200;

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    /* Use 128‑bit to avoid overflow even for large r. */
    while ( ((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n )
        ++r;

    /* Column index within the row */
    idx_t col = n - ((unsigned __int128)r * (r + 1)) / 2;

    /* Handle very small rows directly */
    if (r == 0 || r == 1)
        return (seq_t)1;

    /* Allocate three small contiguous rows on the stack. */
    /* All rows have at most MAX_ROW+1 elements.          */
    seq_t row_a[MAX_ROW + 1]; /* row n-2   */
    seq_t row_b[MAX_ROW + 1]; /* row n-1   */
    seq_t row_c[MAX_ROW + 1]; /* current row */

    /* Initial rows 0 and 1 */
    row_a[0] = (seq_t)1;          /* row 0 */
    row_b[0] = (seq_t)1;          /* row 1 */
    row_b[1] = (seq_t)1;

    /* Compute rows 2 .. r */
    for (idx_t i = 2; i <= r; ++i)
    {
        /* First and last entries are 1 */
        row_c[0] = (seq_t)1;
        row_c[i] = (seq_t)1;

        for (idx_t k = 1; k < i; ++k)
        {
            /* Unpack constants for readability */
            seq_t term1 = (seq_t)2 * (seq_t)(i - k) + (seq_t)1;    /* 2*(n-k)+1 */
            seq_t term2 = (seq_t)2 * (seq_t)k      + (seq_t)1;    /* 2*k+1   */
            seq_t term3 = (seq_t)2 * (seq_t)k * (seq_t)(i - k);   /* 2*k*(n-k) */

            seq_t a_val = row_b[k - 1];      /* T(i-1,k-1,2) */
            seq_t b_val = row_b[k];          /* T(i-1,k,2)   */
            seq_t c_val = row_a[k - 1];      /* T(i-2,k-1,2) */

            /* Compute T(i,k,2) */
            row_c[k] = term1 * a_val + term2 * b_val - term3 * c_val;
        }

        /* Rotate rows for next iteration:
         *   row_a <- row_b
         *   row_b <- row_c
         */
        /* Copy row_b into row_a */
        for (idx_t j = 0; j <= i - 1; ++j)
            row_a[j] = row_b[j];
        /* Copy row_c into row_b */
        for (idx_t j = 0; j <= i; ++j)
            row_b[j] = row_c[j];
    }

    /* Row r is now in row_b.  Return the requested column. */
    return row_b[col];
}
