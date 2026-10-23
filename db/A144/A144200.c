/*
 * A144200  Square array (8 X 8) read by rows.
 * https://oeis.org/A144200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 1 if (r,c) ∈
 *            {(0,7),(1,1),(2,2),(3,0),(4,4),(5,5),(6,6),(7,3)}; where r =
 *            (n‑1)/8, c = (n‑1)%8; otherwise a(n) = 0. The values come from
 *            the fixed 8×8 permutation matrix described in OEIS entry A144200
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert 1‑based index to 0‑based row and column. */
    idx_t r = (n - 1) / 8;   /* row index, 0‑based */
    idx_t c = (n - 1) % 8;   /* column index, 0‑based */

    /* Only the first 64 terms correspond to the 8×8 array.  For n>64
       we return 0, as the OEIS sequence has no specified terms beyond
       the 64 entries. */
    if (r >= 8)
        return (seq_t)0;

    /* Check for the eight positions containing a 1. */
    if ((r == 0 && c == 7) ||
        (r == 1 && c == 1) ||
        (r == 2 && c == 2) ||
        (r == 3 && c == 0) ||
        (r == 4 && c == 4) ||
        (r == 5 && c == 5) ||
        (r == 6 && c == 6) ||
        (r == 7 && c == 3))
        return (seq_t)1;
    else
        return (seq_t)0;
}
