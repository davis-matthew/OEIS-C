/*
 * A169590  Triangle T(n,k) with : column n = A000034 if n even and column n =
 *          A000007 if n odd.
 * https://oeis.org/A169590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if (row is even and column is even) or (row is odd and
 *            column == row); a(n) = 2 if (row is odd, column is even, and
 *            column != row); a(n) = 0 otherwise. The triangle is enumerated
 *            row by row, left to right, and this function returns the value
 *            at the flattened index n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((seq_t)(r + 1) * (r + 2) / 2) <= (seq_t)n) {
        ++r;
    }

    /* Offset of the first element of row r */
    idx_t offset = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t col = n - offset;

    seq_t value = 0;

    if ((r & 1) == 0) {            /* even row */
        if ((col & 1) == 0)
            value = (seq_t)1;
    } else {                       /* odd row */
        if (col == r)
            value = (seq_t)1;
        else if ((col & 1) == 0)
            value = (seq_t)2;
    }

    return value;
}
