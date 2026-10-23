/*
 * A014530  List of sizes of squares occurring in lowest order example of a
 *          perfect squared square.
 * https://oeis.org/A014530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014530: sizes of squares in the lowest order perfect squared square.
   The sequence is finite (21 terms).  For n outside the range 1…21 this function
   returns 0. */
seq_t a(idx_t n)
{
    static const seq_t vals[21] = {
        2, 4, 6, 7, 8, 9, 11, 15, 16, 17,
        18, 19, 24, 25, 27, 29, 33, 35, 37, 42, 50
    };
    if (n == 0 || n > 21)
        return 0;
    return vals[n - 1];
}
