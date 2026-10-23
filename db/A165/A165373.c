/*
 * A165373  Number of slanted n X 4 (i=1..n) X (j=i..4+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, and 4 in the lower right
 *          corner.
 * https://oeis.org/A165373
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (64*n^4 - 320*n^3 + 716*n^2 - 1060*n + 1107) / 3 for n >
 *            2, with a(2) = 37. (Offset 2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 94, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices < 2 just to have defined behaviour.  */
    if (n < 2) return 0;

    if (n == 2) return 37;

    /* Compute the polynomial using only integer arithmetic.  */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */
    seq_t n4 = n3 * nn;          /* n^4   */

    seq_t term = 64 * n4
               - 320 * n3
               + 716 * n2
               - 1060 * nn
               + 1107;

    /* The expression is always divisible by 3. */
    return term / 3;
}
