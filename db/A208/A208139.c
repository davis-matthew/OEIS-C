/*
 * A208139  Number of n X 5 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 4*n^4 + 5*n^3 + 2*n^2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* ensure calculations are in seq_t */
    seq_t n2 = nn * nn;           /* n^2  */
    seq_t n3 = n2 * nn;           /* n^3  */
    seq_t n4 = n3 * nn;           /* n^4  */
    seq_t n5 = n4 * nn;           /* n^5  */
    return n5 + (seq_t)4 * n4 + (seq_t)5 * n3 + (seq_t)2 * n2;
}
