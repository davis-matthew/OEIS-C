/*
 * A123154  A 3 X 3 magic square read by rows. From a quiz.
 * https://oeis.org/A123154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, tabf
 * Formula:   a(n) repeats the 9‐digit block 6,7,2,1,5,9,8,3,4 (magic square
 *            read by rows)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t block[9] = {6, 7, 2, 1, 5, 9, 8, 3, 4};
    return block[(n - 1) % 9];
}
