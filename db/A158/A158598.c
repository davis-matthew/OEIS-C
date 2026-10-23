/*
 * A158598  a(n) = 40*n^2 - 1.
 * https://oeis.org/A158598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 */
    if (n == 0) return 0;          /* undefined for the sequence */
    seq_t nn = (seq_t)n;
    /* Compute 40 * n^2 - 1 */
    return 40 * nn * nn - 1;
}
