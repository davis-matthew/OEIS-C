/*
 * A097693  Largest achievable determinant of a 3 X 3 matrix whose elements
 *          are 9 distinct integers chosen from the range -n...n.
 * https://oeis.org/A097693
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ( 2*n^3 - 7*n^2 + 6*n + 3 )
 * Range:     u8 n <= 5, u16 n <= 26, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once to avoid repeated casts. */
    seq_t nn = (seq_t)n;
    /* Compute the cubic polynomial in a single expression. */
    seq_t res = 2 * ( 2 * nn * nn * nn - 7 * nn * nn + 6 * nn + 3 );
    return res;
}
