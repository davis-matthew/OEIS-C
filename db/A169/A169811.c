/*
 * A169811  a(n) = n XOR n*(n+1)/2.
 * https://oeis.org/A169811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the triangular number t = n*(n+1)/2 in the seq_t type. */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Return n XOR t, with n promoted to seq_t for the bitwise operation. */
    return ((seq_t)n) ^ tri;
}
