/*
 * A064883  Eisenstein array Ei(1,3).
 * https://oeis.org/A064883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   ei_eval
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Eisenstein array Ei(1,3) – sequence A064883
 *  a(1,0) = 1,  a(1,1) = 3
 *  a(n,m) = a(n-1, m/2)                       if m is even
 *          = a(n-1, (m-1)/2) + a(n-1, (m+1)/2)  if m is odd
 *
 *  The one‑dimensional sequence is obtained by reading the rows
 *  of the two‑dimensional array left‑to‑right, starting with row 1.
 *
 *  This implementation locates the row and column that correspond to the
 *  requested index and evaluates the recurrence recursively.
 */

#include <stdint.h>
#include "../utils/ei_eval.h"
/* seq_t and idx_t are supplied by the build environment; do not define them here. */

seq_t a(idx_t n) {
    /* Find the row that contains the n-th element. */
    idx_t offset = 0;       /* total elements in all preceding rows   */
    idx_t length = 2;       /* number of elements in the current row  */
    idx_t row = 1;          /* current row number (1‑based)           */

    /* Advance rows until the cumulative length covers n. */
    while (offset + length < n) {
        offset += length;
        ++row;
        /* length doubles and subtracts one for the next row: 2^k+1 -> 2^(k+1)+1 */
        length = (length << 1) - 1;
    }

    /* Column index within the found row (0‑based). */
    idx_t col = n - offset - 1;

    return ei_eval(row, col);
}
