/*
 * A127780  A127775 * A002260 as infinite lower triangular matrices.
 * https://oeis.org/A127780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2*row-1)*(n - tbl(row-1)) where row is the smallest *
 *            integer with t(row) = row*(row+1)/2 >= n. Here tbl(row-1) =
 *            (row-1)*row/2. * This generates the triangle with entries
 *            (2*row-1)*k for row≥1, 1≤k≤row, * read row‑by‑row
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrt */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; n=0 is undefined, but we return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Find the row.  Compute r0 = floor((sqrt(8*n+1)-1)/2) and then
       increment until the triangular number exceeds or equals n. */
    double nd = (double)n;
    idx_t r = (idx_t)((sqrt(8.0 * nd + 1.0) - 1.0) / 2.0);
    /* If r*(r+1)/2 < n, increment until it holds. */
    while ((seq_t)r * (r + 1) / 2 < n) ++r;

    /* Number of elements before this row = (r-1)*r/2. */
    seq_t prev_count = (seq_t)(r - 1) * r / 2;

    /* Position within the row (1‑based). */
    seq_t pos = (seq_t)n - prev_count;

    /* Value = (2*r-1) * pos. */
    seq_t val = ((seq_t)(2 * r - 1)) * pos;
    return val;
}
