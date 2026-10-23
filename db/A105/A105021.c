/*
 * A105021  Prime form of first twelve-tone row created by Arnold Schoenberg
 *          from the last movement of the Five Piano Pieces, op. 23. A sketch
 *          of this Waltz is dated Jul 26, 1921.
 * https://oeis.org/A105021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = [0,8,10,6,7,5,9,1,3,2,11,4][n mod 12]
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t row[12] = {0, 8, 10, 6, 7, 5, 9, 1, 3, 2, 11, 4};
    return row[n % 12];
}
