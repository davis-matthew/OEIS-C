/*
 * A144199  Square array (8 X 8) read by rows.
 * https://oeis.org/A144199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n is the linear index (row‑major) of a 1 in the 8×8 permutation matrix
   with rows: (1,8), (2,1), (3,3), (4,4), (5,5), (6,6), (7,7), (8,2); 
   otherwise a(n) = 0.  For n > 64 the function returns 0. */
seq_t a(idx_t n) {
    /* Compute the row (1‑based) and column (1‑based) of the n‑th entry */
    idx_t r = (n - 1) / 8 + 1;
    idx_t c = (n - 1) % 8 + 1;

    /* Outside the 8×8 array – value is 0 */
    if (r > 8) return (seq_t)0;

    /* Test whether this cell holds the single '1' */
    if ((r == 1 && c == 8) ||
        (r == 2 && c == 1) ||
        (r == 3 && c == 3) ||
        (r == 4 && c == 4) ||
        (r == 5 && c == 5) ||
        (r == 6 && c == 6) ||
        (r == 7 && c == 7) ||
        (r == 8 && c == 2))
        return (seq_t)1;
    else
        return (seq_t)0;
}
