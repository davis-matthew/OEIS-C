/*
 * A144194  Square array (6 X 6) read by rows.
 * https://oeis.org/A144194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 1 if n is the linear index of a 1 in the 6×6 permutation
 *            matrix representing the transposition (1 6); otherwise a(n) = 0
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The matrix is:
          0 0 0 0 0 1
          0 1 0 0 0 0
          0 0 1 0 0 0
          0 0 0 1 0 0
          0 0 0 0 1 0
          1 0 0 0 0 0
       and the entries are read row‑wise. */
    if (n < 1)
        return (seq_t)0;
    idx_t row = (n - 1) / 6 + 1;   /* 1‑based row number */
    idx_t col = (n - 1) % 6 + 1;   /* 1‑based column number */

    /* 1s appear only at the following positions:
       (1,6), (2,2), (3,3), (4,4), (5,5), (6,1). */
    if ((row == 1 && col == 6) ||
        (row == 6 && col == 1) ||
        (row == col && row >= 2 && row <= 5))
        return (seq_t)1;
    return (seq_t)0;
}
