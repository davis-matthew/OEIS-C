/*
 * A022170  Triangle of Gaussian binomial coefficients [ n,k ] for q = 6.
 * https://oeis.org/A022170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the Gaussian binomial coefficient
 *            triangle for q = 6, flattened row by row. Triangle recurrence:
 *            T(n,0) = T(n,n) = 1 T(n,k) = T(n-1,k-1) + 6^k * T(n-1,k) for 1 ≤
 *            k < n Then a(n) = T(r,k) where r is the row containing index n
 *            and k is the column within that row: r = max...
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 38, u64 n <= 59, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r by binary search (r ≤ 500 for n up to 10 000) */
    idx_t low = 0, high = 500;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* mid = ceil((low+high)/2) */
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t base = r * (r + 1) / 2;
    idx_t k    = n - base;                        /* column index */

    /* compute triangle rows up to r */
    /* 500 is well above the maximum needed for n ≤ 10 000 (r ≈ 141) */
    seq_t prev[500];
    seq_t curr[500];
    prev[0] = 1;                                  /* row 0 */

    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = 1;                              /* left boundary */
        curr[row] = 1;                            /* right boundary */

        seq_t pow = 6;                            /* 6^1 for the first inner column */
        for (idx_t col = 1; col < row; ++col) {
            curr[col] = prev[col - 1] + pow * prev[col];
            pow = pow * 6;                       /* advance power of 6 */
        }

        /* copy current row into prev for next iteration */
        for (idx_t col = 0; col <= row; ++col) {
            prev[col] = curr[col];
        }
    }

    return prev[k];                              /* answer for index n */
}
