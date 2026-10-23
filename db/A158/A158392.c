/*
 * A158392  a(n) = 676*n^2 - 2*n.
 * https://oeis.org/A158392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 676*n^2 - 2*n
 * Range:     u8 none, u16 n <= 9, u32 n <= 2520, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 676 * n^2 - 2 * n using seq_t arithmetic. */
    seq_t nn = (seq_t)n;
    return nn * nn * (seq_t)676 - (seq_t)2 * nn;
}
