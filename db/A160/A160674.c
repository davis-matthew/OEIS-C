/*
 * A160674  A bisection of A063522.
 * https://oeis.org/A160674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^3 + 30*n^2 + 12*n + 1
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type once */
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;          /* n^2   */
    seq_t n3 = n2 * x;         /* n^3   */

    /* compute the cubic polynomial */
    return 20 * n3 + 30 * n2 + 12 * x + 1;
}
