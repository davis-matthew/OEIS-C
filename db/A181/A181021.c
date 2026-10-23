/*
 * A181021  Maximum number of 1s in an nX(n+2) binary matrix with no three 1s
 *          adjacent in a line along a row, column or diagonally.
 * https://oeis.org/A181021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n+2)(n+1)/2 - floor((n+2)/3), with the special cases
 *            a(2)=6 and a(5)=20
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small exceptions explicitly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    if (n == 5) return (seq_t)20;

    /* Common case: a(n) = (n+2)(n+1)/2 - floor((n+2)/3). */
    seq_t half = ((seq_t)(n + 2) * (seq_t)(n + 1)) / 2;
    seq_t diff = (seq_t)(n + 2) / 3;   /* floor division */
    return half - diff;
}
