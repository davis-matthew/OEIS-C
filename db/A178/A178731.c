/*
 * A178731  a(n) = n XOR 5n, where XOR is bitwise XOR.
 * https://oeis.org/A178731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR 5n
 * Range:     u8 n <= 51, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n ^ (5*n) using only seq_t arithmetic */
seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* Convert index to seq_t */
    return (tn ^ (tn * ((seq_t)5)));
}
