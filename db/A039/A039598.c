/*
 * A039598  Triangle formed from odd-numbered columns of triangle of
 *          expansions of powers of x in terms of Chebyshev polynomials
 *          U_n(x). Sometimes called Catalan's triangle.
 * https://oeis.org/A039598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 189, u64 n <= 630,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the entry obtained by reading the Catalan triangle T(r,k)
 * row‑by‑row.  For n ≥ 0, let r be the greatest integer with
 * r(r+1)/2 ≤ n, and let k = n - r(r+1)/2 (0 ≤ k ≤ r).  Then
 *
 *   a(n) = T(r,k)
 *
 * where T is defined by
 *   T(0,0) = 1,
 *   T(r,0)   = 2*T(r-1,0) + T(r-1,1)              (r > 0)
 *   T(r,k)   = T(r-1,k-1) + 2*T(r-1,k) + T(r-1,k+1)
 *              for 1 ≤ k < r  (r > 0)
 *   T(r,r)   = T(r-1,r-1)                         (r > 0)
 *
 * The sequence begins: 1, 2, 1, 5, 4, 1, 14, 14, 6, 1, 42, 48, 27, 8, 1, ...
 */

#include <stdint.h>

#define MAX_ROW 2000   /* sufficient for n up to at least 10^4 */

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;  /* column index */

    /* Arrays holding the previous and current rows of the triangle.
     * The maximum needed row for n ≤ 10^4 is < 150, so a size of 2000
     * is more than safe. */
    static seq_t prev[MAX_ROW];
    static seq_t cur[MAX_ROW];

    /* Initialize row 0 */
    prev[0] = (seq_t)1;
    if (r == 0)
        return prev[k];                 /* k must be 0 */

    /* Build rows up to r */
    for (idx_t row = 1; row <= r; ++row) {
        /* First element of the row */
        seq_t val0 = prev[0] * 2;
        if (row - 1 >= 1)                /* prev[1] exists only if row-1 ≥ 1 */
            val0 += prev[1];
        cur[0] = val0;

        /* Middle elements (if any) */
        for (idx_t col = 1; col < row; ++col) {
            seq_t left  = prev[col - 1];
            seq_t mid   = prev[col] * 2;
            seq_t right = (col + 1 <= row - 1) ? prev[col + 1] : (seq_t)0;
            cur[col] = left + mid + right;
        }

        /* Last element of the row */
        cur[row] = prev[row - 1];

        /* If we've reached the desired row, return the requested element */
        if (row == r)
            return cur[k];

        /* Prepare for next iteration */
        for (idx_t i = 0; i <= row; ++i)
            prev[i] = cur[i];
    }

    /* Should never reach here */
    return 0;
}
