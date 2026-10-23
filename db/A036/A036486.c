/*
 * A036486  a(n) = ceiling((n^3)/2).
 * https://oeis.org/A036486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ceil(n^3 / 2) = (n^3 + (n mod 2)) / 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t cube   = n_seq * n_seq * n_seq;
    seq_t parity = n_seq & (seq_t)1; /* n mod 2 */
    return (cube + parity) / (seq_t)2;
}
