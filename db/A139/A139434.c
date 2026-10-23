/*
 * A139434  Frieze pattern with 4 rows, read by diagonals.
 * https://oeis.org/A139434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence A139434. Period 20: [1,1,1,1,1,2,3,1,1,2,1,1,1,1,2,1,1,3,2,1] */
seq_t a(idx_t n)
{
    static const seq_t period[20] = {
        1, 1, 1, 1, 1, 2, 3, 1, 1, 2,
        1, 1, 1, 1, 2, 1, 1, 3, 2, 1
    };
    return period[(size_t)(n % 20)];
}
