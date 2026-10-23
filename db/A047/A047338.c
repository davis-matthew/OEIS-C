/*
 * A047338  Numbers that are congruent to {1, 2, 3, 4} mod 7.
 * https://oeis.org/A047338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3*floor((n-1)/4)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to the sequence type */
    seq_t k     = (n_seq - 1) / 4;   /* floor((n-1)/4) using integer division */
    return n_seq + 3 * k;            /* a(n) = n + 3*k */
}
