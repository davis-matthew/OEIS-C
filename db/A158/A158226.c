/*
 * A158226  a(n) = 225*n^2 - 2*n.
 * https://oeis.org/A158226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n^2 - 2*n
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 4369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 225*n^2 - 2*n using seq_t arithmetic */
    seq_t nn = (seq_t)n;
    seq_t term = ((seq_t)225 * nn) * nn - ((seq_t)2 * nn);
    return term;
}
