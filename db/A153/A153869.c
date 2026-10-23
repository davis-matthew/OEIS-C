/*
 * A153869  Triangle read by rows, A129186 * A128064(unsigned).
 * https://oeis.org/A153869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: for r ≥ 3 a row has zeros everywhere except
   columns r‑2 and r‑1 contain the values r‑2 and r‑1.  The first two rows
   are [1] and [1,0] respectively.  a(n) is the element at the n‑th
   position when the triangle is read row by row. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* safety for an offset < 1   */
        return (seq_t)0;

    /* Find the row r such that the n‑th position lies in that row. */
    idx_t r = 1;
    idx_t tri = 1;                /* triangular number r*(r+1)/2    */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }

    /* Compute the column (1‑based) within that row. */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t col = n - prev_tri;     /* 1 ≤ col ≤ r                    */

    /* Return the appropriate value.  All arithmetic for the
       returned value is done in seq_t. */
    if (r == 1)
        return (seq_t)1;

    if (r == 2) {                 /* second row: [1,0]              */
        if (col == 1)
            return (seq_t)1;
        else
            return (seq_t)0;
    }

    /* For rows r ≥ 3.  Non‑zero entries occur only at columns
       r‑2 and r‑1.                                            */
    if (col == r - 2)
        return (seq_t)(r - 2);
    if (col == r - 1)
        return (seq_t)(r - 1);

    return (seq_t)0;               /* all other positions are zero   */
}
