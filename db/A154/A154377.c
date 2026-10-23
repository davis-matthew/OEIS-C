/*
 * A154377  a(n) = 25*n^2 + 2*n.
 * https://oeis.org/A154377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 + 2*n
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Handles any positive integer n (offset 1). */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;   /* cast index to sequence type */
    /* compute 25*n^2 + 2*n safely using seq_t arithmetic */
    return 25 * nn * nn + 2 * nn;
}
