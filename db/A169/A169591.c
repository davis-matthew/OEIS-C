/*
 * A169591  Triangle T(n,k) with column n = A059841 if n even and column n =
 *          A000007 if n odd.
 * https://oeis.org/A169591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattening (row major order) of the triangle T(r,c)
 *            where column n is A059841 for n even and A000007 for n odd. The
 *            entry is T(r,c) = 1 if (c even and r even) or (c odd and c==r) 0
 *            otherwise a(n) = T(r,c) for the unique (r,c) with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r containing index n.  */
    idx_t row  = 0;
    idx_t sum  = 0;
    while (sum + row + 1 <= n) {
        sum += row + 1;
        row++;
    }

    /* Column index within the row.  */
    idx_t col = n - sum;

    /* Compute the triangle entry.  */
    if ((col & 1) == 0) {          /* even column */
        return (row & 1) ? 0 : 1;
    } else {                       /* odd column */
        return (col == row) ? 1 : 0;
    }
}
