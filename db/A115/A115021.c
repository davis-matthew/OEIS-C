/*
 * A115021  Numbers typed on a keyboard's numeric keypad: start at 1 and read
 *          alternately up and down until reaching 9.
 * https://oeis.org/A115021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = if n<9, the nth element of the snake traversal of the 3×3
 *            numeric keypad. For 0≤n<9: let col = n/3 (column index 0..2) let
 *            pos = n%3 (position within that column, 0..2) if col is even:
 *            row = 2 - pos (bottom→top) else: row = pos (top→bottom) a(n) =
 *            (2 - row) * 3 + 1 + col For n ≥ 9 the sequ...
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n >= 9) return 0;          /* sequence ends at index 8 */

    idx_t col = n / 3;             /* column 0,1,2 */
    idx_t pos = n % 3;             /* 0,1,2 */

    idx_t row;
    if ((col & 1) == 0)           /* even column: bottom→top   */
        row = 2 - pos;
    else                          /* odd column:   top→bottom */
        row = pos;

    /* compute key number: (2 - row) * 3 + 1 + col */
    seq_t key = (seq_t)( (2 - row) * 3 + 1 + col );
    return key;
}
