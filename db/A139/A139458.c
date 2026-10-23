/*
 * A139458  Frieze pattern of order 6, with 5 rows, read by diagonals.
 * https://oeis.org/A139458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = period[n mod 30] where period =
 *            {1,1,1,1,1,1,2,3,4,1,1,2,3,1,1,1,2,1,2,1,1,1,3,2,1,1,4,3,2,1}
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t period[30] = {
        1, 1, 1, 1, 1, 1,
        2, 3, 4, 1, 1, 2,
        3, 1, 1, 1, 2, 1,
        2, 1, 1, 1, 3, 2,
        1, 1, 4, 3, 2, 1
    };
    return period[n % 30];
}
