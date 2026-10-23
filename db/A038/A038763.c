/*
 * A038763  Triangular matrix arising in enumeration of catafusenes, read by
 *          rows.
 * https://oeis.org/A038763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the entry T(r,k) of the triangular array defined by
 *            T(0,0) = 1; T(n,0) = 1 (n ≥ 1); T(1,1) = 1; T(n,k) =
 *            3*T(n-1,k-1) + T(n-1,k) (n ≥ 2, 1 ≤ k ≤ n); a(n) = T(r,k) where
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 60, u32 n <= 182, u64 n <= 618,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* determine the row r and column k that contain index n   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* The largest possible r for n ≤ 10000 is < 140, 
       so a fixed array of a few hundred elements is enough. */
    seq_t prev[202] = {0};
    seq_t cur[202]  = {0};

    /* Row 0 */
    prev[0] = 1;
    if (r == 0)
        return prev[0];

    /* Row 1 (special initialization) */
    cur[0] = 1;
    cur[1] = 1;
    prev[0] = cur[0];
    prev[1] = cur[1];
    if (r == 1)
        return prev[k];   /* k is 0 or 1 here */

    /* Compute rows 2 … r using the recurrence */
    for (idx_t i = 2; i <= r; ++i) {
        cur[0] = 1;               /* T(i,0) = 1 */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t t = prev[j - 1] * 3;
            if (j <= i - 1)
                t += prev[j];
            cur[j] = t;
        }
        /* copy current row into prev for the next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = cur[j];
    }

    return prev[k];
}
