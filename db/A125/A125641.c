/*
 * A125641  Square of the (3,1)-entry of the 3 X 3 matrix M^n, where M =
 *          [1,0,0; 1,1,0; 1,i,1].
 * https://oeis.org/A125641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 - 2*n + 5)/4
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 265, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* convert index to seq_t */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t inner = n2 + (seq_t)5 - (seq_t)2 * nn;  /* n^2 - 2n + 5 */
    return n2 * inner / (seq_t)4;   /* divide by 4 */
}
