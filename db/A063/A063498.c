/*
 * A063498  Number of atoms in cluster of n layers around C_60.
 * https://oeis.org/A063498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*n^3/3 + 10*n^2 + 56*n/3
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 759, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;
    seq_t n_cu  = n_sq * n_seq;
    seq_t numerator = (seq_t)10 * n_cu + (seq_t)30 * n_sq + (seq_t)56 * n_seq;
    return numerator / (seq_t)3;
}
