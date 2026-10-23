/*
 * A167557  The lower left triangle of the ED1 array A167546.
 * https://oeis.org/A167557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 4^(m-1)*(m-1)!*(n+m-2)!/(2*m-2)! where m is the column
 *            index in the lower‑left triangle of the ED1 array A167546, and
 *            the linear index n runs row by row (row 1 has one element, row 2
 *            has two, …)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 51, u64 n <= 119, u128 n <= 345,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n=0 is not used */
        return 0;

    /* Find the row r such that r*(r-1)/2 < n <= r*(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column index m (1‑based). */
    idx_t m = n - r * (r - 1) / 2;   /* 1 <= m <= r */

    /* Compute the value using the product form to avoid large intermediate
       factorials.  a = 4^(m-1) * (m-1)! * Π_{i=1}^{n-m} (2m-2 + i). */
    seq_t res = 1;

    /* Multiply (m-1)! and 4^(m-1) together. */
    for (idx_t i = 1; i <= m - 1; ++i) {
        res *= (seq_t)i;
        res *= 4;
    }

    /* Multiply the remaining factors. */
    for (idx_t i = 1; i <= r - m; ++i) {
        res *= (seq_t)(2 * m - 2 + i);
    }

    return res;
}
