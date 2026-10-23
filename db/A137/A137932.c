/*
 * A137932  Terms in an n X n spiral that do not lie on its principal
 *          diagonals.
 * https://oeis.org/A137932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n - (2*n - (n & 1))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n^2 - 2*n + (n & 1) */
    return (nn * nn) - (((seq_t)2 * nn) - (seq_t)(n & 1));
}
