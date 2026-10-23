/*
 * A144196  Square array (6 X 6) read by rows.
 * https://oeis.org/A144196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144196
   A 6×6 permutation matrix flattened row‑wise.
   The mapping of rows to columns is:
   row 1 → col 6
   row 2 → col 2
   row 3 → col 1
   row 4 → col 4
   row 5 → col 5
   row 6 → col 3
   The nth term is 1 when the column index of the cell equals the
   mapping for its row; otherwise it is 0.  For n>36 all terms are 0. */

#include <stdint.h>

static const idx_t perm[7] = {0, 6, 2, 1, 4, 5, 3};

seq_t a(idx_t n) {
    /* Determine the 0‑based indices of the row and column in the 6×6 array */
    idx_t r = (n - 1) / 6 + 1;   /* row number (1..6) */
    idx_t c = (n - 1) % 6 + 1;   /* column number (1..6) */

    /* If the row is beyond the 6×6 array, the entry is 0 */
    if (r > 6) return 0;

    /* Return 1 if the column matches the permutation mapping, else 0 */
    return (perm[r] == c) ? 1 : 0;
}
