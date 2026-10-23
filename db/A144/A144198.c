/*
 * A144198  Square array (8 X 8) read by rows.
 * https://oeis.org/A144198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 1 if the entry of the 8×8 permutation matrix is 1,
 *            otherwise 0 The matrix has 1's at (1,8), (8,1), and (i,i) for 2
 *            ≤ i ≤ 7. All other entries are 0. The sequence is the rows read
 *            consecutively; terms after the 64th are implicitly 0
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to 0-based */
    idx_t idx = n - 1;
    /* Compute row (1‑based) and column (1‑based) in the 8×8 matrix */
    idx_t row = idx / 8 + 1;
    idx_t col = idx % 8 + 1;

    if ((row == 1 && col == 8) ||
        (row == 8 && col == 1) ||
        (row == col && row >= 2 && row <= 7))
    {
        return (seq_t)1;
    }
    else
    {
        return (seq_t)0;
    }
}
