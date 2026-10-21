/*
 * A093361  Add/multiply sequence, see example.
 * https://oeis.org/A093361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*(4*n^3 + 12*n^2 + 20*n + 33 + (6*n^2 - 9)*(-1)^n)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified computation: for even n use 4*n^3 + 18*n^2 + 20*n + 24,
   for odd  n use 4*n^3 + 6*n^2 + 20*n + 42.  All operations are on seq_t. */
seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;          /* n^2   */
    seq_t n3 = n2 * ns;          /* n^3   */
    seq_t num;

    if ((n & 1) == 0) {          /* n is even */
        num = n3 * 4 + n2 * 18 + ns * 20 + 24;
    } else {                     /* n is odd  */
        num = n3 * 4 + n2 * 6 + ns * 20 + 42;
    }

    return num / 24;             /* exact division by 24 */
}
