/*
 * A092185  a(n) = (5/6)*n^3+(5/2)*n^2+(8/3)*n.
 * https://oeis.org/A092185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5/6)*n^3 + (5/2)*n^2 + (8/3)*n
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 989, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first */
    seq_t n_seq = (seq_t)n;

    /* Compute numerator: n_seq * (5*n_seq*n_seq + 15*n_seq + 16) */
    seq_t n2 = n_seq * n_seq;
    seq_t numerator = n_seq * (5 * n2 + 15 * n_seq + 16);

    /* Final division by 6 (exact division as per formula) */
    return numerator / 6;
}
