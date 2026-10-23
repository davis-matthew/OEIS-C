/*
 * A121722  Triangle T(n,k) = 1 + k*n*(n+1)/2, read by rows.
 * https://oeis.org/A121722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + (n - r*(r+1)/2) * r*(r+1)/2 * where r is the unique
 *            row index satisfying r*(r+1)/2 <= n < (r+1)*(r+2)/2. * This
 *            implements the triangle T(n,k) = 1 + k*binomial(n+1,2) read by
 *            rows
 * Range:     u8 n <= 43, u16 n <= 1375, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t low = 0;
    idx_t high = n;
    idx_t r = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        /* Use 128‑bit arithmetic to avoid overflow in the intermediate product. */
        unsigned __int128 prod = ((unsigned __int128)mid * ((unsigned __int128)mid + 1)) / 2;
        if (prod <= n)
        {
            r = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;  /* prevent infinite loop when n = 0 */
            high = mid - 1;
        }
    }

    /* Compute binomial(r+1,2) = r*(r+1)/2 safely in 128‑bit then cast to seq_t. */
    seq_t R = (seq_t)r;
    seq_t offset = (seq_t)(((unsigned __int128)R * ((unsigned __int128)R + 1)) / 2);

    /* k = n - offset (the column index in row r).  */
    seq_t k = (seq_t)n - offset;

    /* Triangle entry: 1 + k * offset.  */
    return 1 + k * offset;
}
