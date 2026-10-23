/*
 * A144193  Square array (5 X 5) read by rows.
 * https://oeis.org/A144193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) is 1 for the first row last column, the last row first
 *            column, and the diagonal entries (2,2), (3,3), (4,4) of a 5×5
 *            matrix; all other entries are 0. For n > 25 the sequence is 0
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t arr[25] = {
        0, 0, 0, 0, 1,   /* row 1 */
        0, 1, 0, 0, 0,   /* row 2 */
        0, 0, 1, 0, 0,   /* row 3 */
        0, 0, 0, 1, 0,   /* row 4 */
        1, 0, 0, 0, 0    /* row 5 */
    };
    if (n <= 25) {
        /* n is 1-based; array index is 0-based */
        return arr[n - 1];
    }
    return (seq_t)0;
}
