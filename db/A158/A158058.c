/*
 * A158058  a(n) = 16*n^2 - 2*n.
 * https://oeis.org/A158058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 2*n
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    /* Compute 16*n^2 - 2*n safely in seq_t arithmetic */
    seq_t term = ns * ((seq_t)16 * ns - (seq_t)2);
    return term;
}
