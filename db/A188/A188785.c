/*
 * A188785  Number of 2-step self-avoiding walks on an n X n X n X n 4-cube
 *          summed over all starting positions.
 * https://oeis.org/A188785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*n^4 - 8*n^3 (for n >= 1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;          /* n^3 */
    seq_t n1 = (seq_t)(n - 1);        /* n-1 (n>=1) */
    return 8 * n3 * n1;               /* 8*n^3*(n-1) */
}
