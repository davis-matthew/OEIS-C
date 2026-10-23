/*
 * A144197  Square array 7 x 7 read by rows.
 * https://oeis.org/A144197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if (row,col) equals (1,7),(2,2),(3,3),(4,4),(5,5),(6,6),(7,1); otherwise 0. */

seq_t a(idx_t n)
{
    /* Determine 1‑based row and column from the linear index n. */
    idx_t row = (n - 1) / 7 + 1;   /* row index in [1,7] for n <= 49, larger otherwise */
    idx_t col = (n - 1) % 7 + 1;   /* column index in [1,7] for n <= 49, larger otherwise */

    /* Check if the position contains a 1 in the defined permutation matrix. */
    if ((row == 1 && col == 7) ||          /* transposition (1,7) */
        (row == 7 && col == 1) ||          /* transposition (7,1) */
        (row == col && row >= 2 && row <= 6)) /* fixed points 2..6 */
    {
        return (seq_t)1;
    }
    else
    {
        return (seq_t)0;
    }
}
